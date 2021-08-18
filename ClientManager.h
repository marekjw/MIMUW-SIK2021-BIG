

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "client-state.h"
#include <cstring>
#include <sys/socket.h>

#define BUFFER_SIZE 4096


class ClientManager {
private:
  ClientState state;
  int game_socket, gui_socket;

  unsigned char server_buffer[BUFFER_SIZE]{};

  void events_to_gui();

  [[noreturn]] void gui_to_client();

  [[noreturn]] void client_to_server();

  msg_from_gui read_from_gui();


  /**
   * Sends current state to server - which key is pressed, session_id etc
   */
  void send_to_server();

  void parse_server_buffer(ssize_t size);

  uint32_t parse_game_number();

  void parse_event(ssize_t &counter, ssize_t size);

public:
  ClientManager(ClientState &state, int game_socket, int gui_socket)
      : game_socket(game_socket), gui_socket(gui_socket), state(state) {
    memset(&server_buffer, 0, BUFFER_SIZE);
  };

  void start();

  ~ClientManager();
};

#endif // CLIENTMANAGER_H