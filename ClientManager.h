

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "client-state.h"
#include <cstring>
#include <sys/socket.h>

#define BUFFER_SIZE 4096

enum msg_from_gui {
  LEFT_KEY_UP,
  LEFT_KEY_DOWN,
  RIGHT_KEU_UP,
  RIGHT_KEY_DOWN,
};

class ClientManager {
private:
  ClientState state;
  int game_socket, gui_socket;

  char server_buffer[BUFFER_SIZE]{};

  void events_to_gui();

  [[noreturn]] void gui_to_client();

  [[noreturn]] void client_to_server();

  msg_from_gui read_from_gui();

  void send_to_server();

public:
  ClientManager(ClientState &state, int game_socket, int gui_socket)
      : game_socket(game_socket), gui_socket(gui_socket), state(state) {
    memset(&server_buffer, 0, BUFFER_SIZE);
  };

  void start();

  ~ClientManager();
};

#endif // CLIENTMANAGER_H
