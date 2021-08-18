
#include "ClientManager.h"
#include "client-state.h"
#include "err.h"
#include <chrono>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define SEND_TO_SERVER_T 30

void ClientManager::start() {
  std::thread threads[3];

  threads[0] = std::thread(&ClientManager::events_to_gui, this);
  threads[1] = std::thread(&ClientManager::gui_to_client, this);
  threads[2] = std::thread(&ClientManager::client_to_server, this);

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

    parse_server_buffer(size);
  }
}

[[noreturn]] void ClientManager::gui_to_client() {
  while (true) {
    state.update_direction(read_from_gui());
  }
}

[[noreturn]] void ClientManager::client_to_server() {
  // TODO

  while (true) {
    auto wake_up_at = std::chrono::steady_clock::now() +
                      std::chrono::milliseconds(SEND_TO_SERVER_T);

    send_to_server();

    std::this_thread::sleep_until(wake_up_at);
  }
}
msg_from_gui ClientManager::read_from_gui() {
  // TODO reads from socket from gui
  return LEFT_KEY_DOWN;
}
void ClientManager::send_to_server() {
  std::vector<unsigned char> message;
  // TODO change htonl to 64 bit
  message.push_back(htonl(state.get_session_id()));
  message.push_back(htons(state.get_turn_direction()));
  message.push_back(htonl(state.get_next_expected_event_no()));
  std::copy(state.get_name().begin(), state.get_name().end(),
            std::back_inserter(message));

  if (send(game_socket, message.data(), message.size(), 0) < 0)
    syserr("Could not send datagram to server");
}

ClientManager::~ClientManager() {
  if (close(game_socket) < 0)
    syserr("Cannot close game socket");

  if (close(gui_socket) < 0)
    syserr("Cannot close gui socket");
}

#define GAME_NO_BYTES 4

void ClientManager::parse_server_buffer(ssize_t size) {
  uint32_t game_number = parse_game_number();
  if (state.valid_game_number(0))
    return;

  ssize_t counter = GAME_NO_BYTES;

  while (counter < size)
    parse_event(counter, size);
}

uint32_t ClientManager::parse_game_number() {
  return ntohl(*(reinterpret_cast<uint32_t *>(server_buffer)));
}

void ClientManager::parse_event(ssize_t &counter, ssize_t size) {}
