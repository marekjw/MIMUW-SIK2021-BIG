#include "ServerManager.h"

#include "Events/GameOverEvent.h"
#include "Events/PixelEvent.h"
#include "Events/PlayerEliminatedEvent.h"
#include "Parser.h"

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
    if (state.ready_players() >= 2) {
      state.set_up_new_game();
      game_loop();
      add_event(GameOverEvent());
    }
  }
}

[[noreturn]] void ServerManager::send_events_queue() {
  std::unique_lock<std::mutex> lock(event_mutex);

  do {
    cv.wait(lock, [this] { return next_event_to_send < events.size(); });

    if (next_event_to_send < events.size()) {
      Event *event_pointer = &events[next_event_to_send++];
      lock.unlock();

      send_event_to_all(event_pointer);

      lock.lock();
    }
  } while (true);
}

[[noreturn]] void ServerManager::listener() {
  struct sockaddr_storage from {};
  socklen_t from_len;

  while (true) {
    auto bytes_read =
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (sockaddr *)&from, &from_len);

    if (bytes_read < 0 || MAX_CLIENT_DATAGRAM_LEN < bytes_read)
      continue;

    auto datagram = Parser::parse(buffer, bytes_read);

    // TODO dodać gościa elo
  }
}

void ServerManager::game_loop() {
  while (true) {
    auto wake_up_at = std::chrono::steady_clock::now() +
                      std::chrono::milliseconds(round_time);

    for (auto &player : state.get_players()) {
      // TODO inactive player

      player.update_direction();
      if (!player.move())
        continue;

      if (!state.pixel_valid(player.get_position())) {
        // eliminate the player

        // TODO kill the player

        add_event(PlayerEliminatedEvent(player.get_number()));
        if (state.players_alive() <= 1) {
          // TODO send events queue
          return;
        }
      } else {
        add_event(PixelEvent(player.get_number(), player.get_position()));
      }
    }
    // TODO send events queue
    std::this_thread::sleep_until(wake_up_at);
  }
}
void ServerManager::add_event(Event event) {
  std::unique_lock<std::mutex> lock(event_mutex);
  lock.lock();
  events.push_back(event);
  lock.unlock();
}
