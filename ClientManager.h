

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "ClientState.h"
#include "StreamBuffer.h"
#include "constants.h"
#include <queue>
#include <sys/socket.h>

class ClientManager {
private:
  ClientState state;
  int game_socket, gui_socket;
  unsigned char server_buffer[BUFFER_SIZE]{};
  StreamBuffer gui_buffer;

  void server_to_gui();

  void send_events_to_gui();

  [[noreturn]] void gui_to_client();

  [[noreturn]] void client_to_server();

  msg_from_gui read_from_gui();

  /**
   * Sends current state to server - which key is pressed, session_id etc
   */
  void send_to_server();

  void parse_server_buffer(ssize_t size);

  void parse_event(ssize_t &counter, ssize_t size, bool &crc_ok);

  void parse_new_game_event(ssize_t start, ssize_t end);

  void parse_pixel_event(ssize_t start, ssize_t end);

  void parse_player_eliminated_event(ssize_t start, ssize_t end);

  void handle_game_over_event();

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
