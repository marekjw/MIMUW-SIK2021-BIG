
#include "ClientManager.h"
#include "client-state.h"
#include "err.h"
#include <sys/socket.h>
#include <thread>

void ClientManager::start() {
  std::thread threads[2];

  threads[0] = std::thread(&ClientManager::events_to_gui, this);
  threads[1] = std::thread(&ClientManager::gui_to_client, this);

  for (auto &t : threads)
    t.join();
}
 void ClientManager::events_to_gui() {
  // TODO
  while (true) {

    // read data from the server
    ssize_t size = recv(game_socket, server_buffer, BUFFER_SIZE, 0);
    if (size < 0)
      syserr("Could not read from server socket");

    if (size == 0)
      break;

    // TODO
  }
}

[[noreturn]] void ClientManager::gui_to_client() {
  while (true) {
    // read data from the gui
    switch (read_from_gui()) {

    case LEFT_KEY_UP:
      state.set_turn_direction(STRAIGHT);
      break;
    case LEFT_KEY_DOWN:
      state.set_turn_direction(LEFT_DIRECTION);
      break;
    case RIGHT_KEU_UP:
      state.set_turn_direction(STRAIGHT);
      break;
    case RIGHT_KEY_DOWN:
      state.set_turn_direction(RIGHT_DIRECTION);
      break;
    }
  }
}
void ClientManager::client_to_server() {
  // TODO
}
msg_from_gui ClientManager::read_from_gui() {
  // TODO
  return LEFT_KEY_DOWN;
}
