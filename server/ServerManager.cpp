#include "ServerManager.h"

void ServerManager::start() {
  std::thread threads[] = {std::thread(&ServerManager::main_game_loop, this),
                           std::thread(&ServerManager::send_events_queue, this),
                           std::thread(&ServerManager::listener, this)};

  for (auto &t : threads)
    t.join();
}
void ServerManager::main_game_loop() {}

void ServerManager::send_events_queue() {}

void ServerManager::listener() {}
