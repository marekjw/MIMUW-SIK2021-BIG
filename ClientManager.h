

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "StreamBuffer.h"
#include "client-state.h"
#include "constants.h"
#include <sys/socket.h>

class ClientManager {
private:
  ClientState state;
  int game_socket, gui_socket;
  unsigned char server_buffer[BUFFER_SIZE]{};
  StreamBuffer gui_buffer;

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
  ClientManager(ClientState &state, StreamBuffer &gui_buffer, int game_socket,
                int gui_socket)
      : game_socket(game_socket), gui_socket(gui_socket), state(state),
        gui_buffer(gui_buffer) {
    memset(&server_buffer, 0, BUFFER_SIZE);
  };

  void start();

  ~ClientManager();
};

#endif // CLIENTMANAGER_H
