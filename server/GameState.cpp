#include "GameState.h"
#include "../util/util.h"

#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
void GameState::set_up_new_game() {
  clients_mutex.lock();
  game_on_mutex.lock();

  std::cerr << "Setting up new game\n";

  game_id = rng();
  the_game_is_on = true;

  players_alive_no = players_sorted.size();

  std::sort(players_sorted.begin(), players_sorted.end(),
            [](const PlayerState &a, const PlayerState &b) {
              return a.get_name() < b.get_name();
            });

  int count = 0;
  for (auto &player : players_sorted) {
    *(player.get_map_pointer()) = &player;
    player.set_position(rng() % max_x + 0.5, rng() % max_y + 0.5, rng() % 360);
    player.set_number(count++);
    if (pixels[player.get_x()][player.get_y()]) {
      pixels[player.get_x()][player.get_y()] = false;
    } else {
      kill_player(player);
    }
  }

  game_on_mutex.unlock();
  clients_mutex.unlock();
}

bool GameState::is_game_on() {
  game_on_mutex.lock();
  bool res = the_game_is_on;
  game_on_mutex.unlock();
  return res;
}

bool GameState::pixel_valid(std::pair<int, int> position) {
  if (position.first < 0 || position.second < 0)
    return false;

  if (pixels.size() <= position.first || pixels[0].size() <= position.second)
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
    std::cerr << "New specator\n";
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
    if (MAX_PLAYERS <= players.size()) {
      std::cerr << "Cannot add new player - player limit would be exceeded\n";
      return false;
    }

    std::cerr << "Adding new player: " << datagram.get_name() << std::endl;
    players_sorted.emplace_back(*from, datagram);
    auto it = players.insert({{res, port}, &players_sorted.back()});
    players_sorted.back().set_map_pointer(&(it.first->second));

    if (datagram.get_turn_direction() != STRAIGHT) {
      players_sorted.back().make_ready();
      game_on_mutex.lock();
      ++ready_players_no;
      game_on_mutex.unlock();
    }

    return true;

  } else {

    // player exists
    if (ptr->second->get_session_id() <= datagram.get_session_id()) {
      ptr->second->stamp();
      ptr->second->set_turn_direction(datagram.get_turn_direction());
      if (ptr->second->get_session_id() < datagram.get_session_id()) {
        ptr->second->update_session_id(datagram.get_session_id());
      }

      game_on_mutex.lock();
      if (!the_game_is_on) {
        if (!ptr->second->is_ready() &&
            datagram.get_turn_direction() != STRAIGHT) {
          ptr->second->make_ready();
          ++ready_players_no;
          std::cerr << "Now there are " << ready_players_no
                    << " players ready\n";
        }
      }
      game_on_mutex.unlock();
      return true;
    }
  }

  return false;
}

unsigned long GameState::ready_players() {
  game_on_mutex.lock();
  auto res = ready_players_no;
  game_on_mutex.unlock();
  return res;
}
unsigned long GameState::players_alive() const { return players_alive_no; }

void GameState::kill_player(PlayerState &player) {
  if (!player.is_alive())
    return;

  std::cerr << "Killing player " << player.get_name() << "\n";

  player.kill();
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
  for (auto it = players.cbegin(); it != players.cend();) {
    if (it->second->should_disconnect()) {
      it->second->disconnect();
      std::cerr << "Disconnect player: " << it->second->get_name() << std::endl;
      it = players.erase(it);
    } else {
      ++it;
    }
  }

  for (auto it = spectators.cbegin(); it != spectators.cend();) {
    if (it->second.should_disconnect()) {
      std::cerr << "Disconnect specatator" << std::endl;
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
             std::all_of(players_sorted.begin(), players_sorted.end(),
                         [](PlayerState &p) { return p.is_ready(); });

  clients_mutex.unlock();
  return res;
}
uint32_t GameState::get_max_x() const { return max_x; }
uint32_t GameState::get_max_y() const { return max_y; }
