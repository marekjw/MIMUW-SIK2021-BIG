#include "GameState.h"

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
void GameState::set_up_new_game() {
  clients_mutex.lock();
  game_on_mutex.lock();

  game_id = rng();
  the_game_is_on = true;

  for (auto &it : players) {
    players_sorted.push_back(&it.second);
  }

  players_alive_no = players_sorted.size();

  std::sort(players_sorted.begin(), players_sorted.end(),
            [](const PlayerState *a, const PlayerState *b) {
              return a->get_name() < b->get_name();
            });

  int count = 0;
  for (auto ptr : players_sorted) {
    double x = (double)(rng() % max_x) + 0.5;
    double y = (double)(rng() % max_y) + 0.5;
    int direction = rng() % 360;

    ptr->set_position(x, y, direction);
    ptr->set_number(count++);
    if (!pixel_valid(ptr->get_position())) {
      kill_player(ptr);
    } else {
      eat_pixel(ptr->get_position());
    }
  }

  game_on_mutex.unlock();
  clients_mutex.unlock();
}

/**
 * Checks if pixel is valid
 * @param position - a pair x, y
 * @return true if the pixel is valid
 */
bool GameState::pixel_valid(std::pair<int, int> position) {
  if (position.first < 0 || position.second < 0)
    return false;

  if (pixels.size() <= (long unsigned int)position.first ||
      pixels[0].size() <= (long unsigned int)position.second)
    return false;

  return pixels[position.first][position.second];
}

void GameState::eat_pixel(std::pair<int, int> pixel) {
  pixels[pixel.first][pixel.second] = false;
}

bool GameState::update_spectator(const Datagram &datagram,
                                 const sockaddr_storage *from) {
  int port;
  char res[INET6_ADDRSTRLEN];

  if (!util::get_ip_str(from, res, port))
    return false;

  clients_mutex.lock();
  bool result = update_spectator_(datagram, from, res, port);
  clients_mutex.unlock();
  return result;
}

bool GameState::update_spectator_(const Datagram &datagram,
                                  const sockaddr_storage *from, char *res,
                                  int port) {
  auto ptr = spectators.find({res, port});

  if (ptr == spectators.end()) {
    // create a new spectator
    spectators.insert({{res, port}, Person{*from, datagram.get_session_id()}});
    return true;
  } else {

    // spectator exists
    if (ptr->second.get_session_id() == datagram.get_session_id()) {
      ptr->second.stamp();
      return true;
    }

    if (ptr->second.get_session_id() < datagram.get_session_id()) {
      ptr->second.update_session_id(datagram.get_session_id());
      ptr->second.stamp();
      return true;
    }
  }

  return false;
}

bool GameState::update_player(const Datagram &datagram,
                              const sockaddr_storage *from) {
  int port;
  char res[INET6_ADDRSTRLEN];

  if (!util::get_ip_str(from, res, port))
    return false;

  clients_mutex.lock();
  bool result = update_player_(datagram, from, res, port);
  clients_mutex.unlock();
  return result;
}

bool GameState::update_player_(const Datagram &datagram,
                               const sockaddr_storage *from, char *res,
                               int port) {

  auto ptr = players.find({res, port});

  if (ptr == players.end()) {

    auto check_name = std::find_if(
        players.begin(), players.end(),
        [&datagram](
            const std::pair<std::pair<std::string, int>, PlayerState> &p) {
          return p.second.get_name() == datagram.get_name();
        });

    if (check_name != players.end()) {
      // there exists a player with given name
      return false;
    }

    bool is_game_on = the_game_is_on;

    if (is_game_on) {
      // cannot add new player during game
      // creating new spectator instead
      return update_spectator_(datagram, from, res, port);
    }

    if (MAX_PLAYERS <= players.size()) {
      return false;
    }

    auto it = players.insert({{res, port}, {*from, datagram}});

    if (datagram.get_turn_direction() != STRAIGHT) {
      it.first->second.make_ready();
      game_on_mutex.lock();
      ++ready_players_no;
      game_on_mutex.unlock();
    }

    return true;

  } else {

    // player exists
    if (ptr->second.get_session_id() <= datagram.get_session_id()) {
      ptr->second.stamp();
      ptr->second.set_turn_direction(datagram.get_turn_direction());
      if (ptr->second.get_session_id() < datagram.get_session_id()) {
        ptr->second.update_session_id(datagram.get_session_id());
      }

      game_on_mutex.lock();
      if (!the_game_is_on) {
        if (!ptr->second.is_ready() &&
            datagram.get_turn_direction() != STRAIGHT) {
          ptr->second.make_ready();
          ++ready_players_no;

        }
      }
      game_on_mutex.unlock();
      return true;
    }
  }

  return false;
}

unsigned long GameState::players_alive() const { return players_alive_no; }

void GameState::kill_player(PlayerState *player) {
  if (!player->is_alive())
    return;

  player->kill();
  --players_alive_no;
}

void GameState::reset() {
  clients_mutex.lock();
  game_on_mutex.lock();

  players_sorted.clear();
  players.clear();
  spectators.clear();

  ready_players_no = 0;
  players_alive_no = 0;
  the_game_is_on = false;

  for (auto &column : pixels)
    column.clear();
  pixels.clear();
  pixels =
      std::vector<std::vector<bool>>(max_x, std::vector<bool>(max_y, true));

  game_on_mutex.unlock();
  clients_mutex.unlock();
}

void GameState::disconnect_inactive_ones() {
  clients_mutex.lock();
  for (auto it = players.begin(); it != players.end();) {
    if (it->second.should_disconnect()) {
      it->second.disconnect();
      game_on_mutex.lock();

      if (it->second.is_ready() && !the_game_is_on) {
        --ready_players_no;
      }
      game_on_mutex.unlock();

      it = players.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = spectators.cbegin(); it != spectators.cend();) {
    if (it->second.should_disconnect()) {
      it = spectators.erase(it);
    } else {
      ++it;
    }
  }

  clients_mutex.unlock();
}

bool GameState::can_start_game() {
  clients_mutex.lock();

  bool res = ready_players_no >= MIN_AMOUNT_OF_PLAYERS &&
             std::all_of(players.begin(), players.end(),
                         [](auto p) { return p.second.is_ready(); });

  clients_mutex.unlock();
  return res;
}
uint32_t GameState::get_max_x() const { return max_x; }
uint32_t GameState::get_max_y() const { return max_y; }
