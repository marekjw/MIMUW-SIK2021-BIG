#include "ServerManager.h"

#include "../util/err.h"
#include "Datagram.h"
#include "Events/GameOverEvent.h"
#include "Events/NewGameEvent.h"
#include "Events/PixelEvent.h"
#include "Events/PlayerEliminatedEvent.h"

#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>

void ServerManager::start() {
  std::thread threads[] = {std::thread(&ServerManager::main_loop, this),
                           std::thread(&ServerManager::listener, this)};

  for (auto &t : threads)
    t.join();
}
[[noreturn]] void ServerManager::main_loop() {
  while (true) {
    auto wake_up_at = std::chrono::steady_clock::now() +
                      std::chrono::milliseconds(round_time);
    state.disconnect_inactive_ones();
    if (state.can_start_game()) {
      state.set_up_new_game();

      add_event(NewGameEvent(events.size(), state.get_players(),
                             state.get_max_y(), state.get_max_x()));

      for (auto &player : state.get_players()) {
        if (player->is_alive()) {
          add_event(PixelEvent(events.size(), player->get_position(),
                               player->get_number()));
        } else {
          add_event(PlayerEliminatedEvent(events.size(), player->get_number()));
        }
      }

      // check if the game is over
      if (state.players_alive() <= MIN_ALIVE_PLAYERS) {
        std::cerr << "GAME ENDED BEFORE IT EVEN STARTED\n";
        add_event(GameOverEvent(events.size()));
        send_events_queue();
      } else {
        send_events_queue();
        game_loop();
      }

      event_mutex.lock();
      state.reset();
      events.clear();
      event_mutex.unlock();
    }
    std::this_thread::sleep_until(wake_up_at);
  }
}

[[noreturn]] void ServerManager::listener() {
  struct sockaddr_storage from {};
  socklen_t from_len = sizeof from;

  std::cerr << "Starting to listen\n";
  while (true) {
    // SOCKET: RECEIVNING THE DATA
    auto bytes_read = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                               (struct sockaddr *)&from, &from_len);

    if (bytes_read < 0) {
      warning("Could not read datagram");
      continue;
    }
    if (MAX_CLIENT_DATAGRAM_LEN < bytes_read) {
      warning("Datagram is too long");
      continue;
    }

    Datagram datagram{buffer, bytes_read};

    if (datagram.invalid_crc() || datagram.invalid_name()) {
      continue;
    }

    if (state.is_game_on() || datagram.get_name().empty()) {
      if (state.update_spectator(datagram, &from)) {
        send_events_to(datagram.get_next_expected_event_no(), &from);
      }
    } else {
      if (state.update_player(datagram, &from)) {
        send_events_to(datagram.get_next_expected_event_no(), &from);
      }
    }
  }
}

void ServerManager::game_loop() {
  while (true) {
    auto wake_up_at = std::chrono::steady_clock::now() +
                      std::chrono::milliseconds(round_time);

    state.disconnect_inactive_ones();

    for (auto &player : state.get_players()) {
      if (!player->is_alive())
        continue;

      player->update_direction();
      if (!player->move())
        continue;

      if (!state.pixel_valid(player->get_position())) {
        // eliminate the player

        state.kill_player(player);

        add_event(PlayerEliminatedEvent(events.size(), player->get_number()));
        // check if the game is over
        if (state.players_alive() <= MIN_ALIVE_PLAYERS) {
          std::cerr << "ONLY 1 PERSON ALIVE\n";
          add_event(GameOverEvent(events.size()));
          send_events_queue();
          return;
        }
      } else {
        state.eat_pixel(player->get_position());
        add_event(PixelEvent(events.size(), player->get_position(),
                             player->get_number()));
      }
    }
    send_events_queue();
    std::this_thread::sleep_until(wake_up_at);
  }
}
void ServerManager::add_event(const Event &event) {
  event_mutex.lock();
  events.push_back(event);
  event_mutex.unlock();
}

void ServerManager::send_events_queue() {
  // TODO może zmienic??

  state.get_map_mutex().lock();

  for (auto &it : state.get_players_map()) {
    if (it.second.is_connected()) {
      send_events_to(next_event_to_send, &it.second.get_address());
    }
  }

  for (auto &it : state.get_spectators_map())
    send_events_to(next_event_to_send, &(it.second.get_address()));

  state.get_map_mutex().unlock();
}

void ServerManager::send_datagram(std::vector<unsigned char> &msg,
                                  const sockaddr_storage *dest) {

  // SOCKET: SENDING THE DATA
  auto res = sendto(sockfd, msg.data(), msg.size(), 0, (struct sockaddr *)dest,
                    sizeof *dest);

  if (res < 0) {
    warning("Could not send datagram");
  }

  msg.clear();
  util::serialize(msg, htonl(state.get_game_id()));
}

void ServerManager::send_events_to(uint32_t next_expected_event,
                                   const sockaddr_storage *destination) {
  std::vector<unsigned char> msg;
  util::serialize(msg, htonl(state.get_game_id()));
  for (auto i = next_expected_event; i < events.size(); ++i) {
    if (msg.size() - GAME_NO_BYTES + events[i].get_data().size() >
        MAX_DATA_FROM_SERVER_LEN) {
      send_datagram(msg, destination);
    }

    msg.insert(msg.end(), events[i].get_data().begin(),
               events[i].get_data().end());
  }

  send_datagram(msg, destination);
}
