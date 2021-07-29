

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "client-state.h"

class ClientManager {
private:
  ClientState state;
  int game_socket, gui_socket;

  [[noreturn]] void client_to_gui();

  void gui_to_client();

  void client_to_server();

  void server_to_client();

public:
  ClientManager(ClientState &state, int game_socket, int gui_socket)
      : game_socket(game_socket), gui_socket(gui_socket), state(state){};

  void start();

  ~ClientManager(){};
};

#endif // CLIENTMANAGER_H
