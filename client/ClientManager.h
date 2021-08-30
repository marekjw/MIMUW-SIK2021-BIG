

#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include "../util/constants.h"
#include "ClientState.h"
#include "StreamBuffer.h"
#include <netdb.h>
#include <queue>
#include <sys/socket.h>

class ClientManager {
private:
  ClientState &state;
  int game_socket, gui_socket;
  addrinfo *server_addr;

  unsigned char server_buffer[BUFFER_SIZE]{};
  StreamBuffer &gui_buffer;

  [[noreturn]] void server_to_gui();

  [[noreturn]] void gui_to_client();

  [[noreturn]] void client_to_server();

  msg_from_gui read_from_gui();

  /**
   * Sends current state to server - which key is pressed, session_id etc
   */
  void send_to_server();

  void parse_server_buffer(ssize_t size);

  void parse_event(ssize_t &counter, ssize_t size, bool &crc_ok);

  void parse_new_game_event(ssize_t start, ssize_t end, uint32_t event_no);

  void parse_pixel_event(ssize_t start, ssize_t end, uint32_t event_no);

  void parse_player_eliminated_event(ssize_t start, ssize_t end,
                                     uint32_t event_no);

  void handle_game_over_event(uint32_t event_no);

public:
  ClientManager(ClientState &client_state, StreamBuffer &gui_buffer, int game_socket,
                int gui_socket, addrinfo *server_addr)
      : game_socket(game_socket), gui_socket(gui_socket), state(client_state),
        gui_buffer(gui_buffer), server_addr(server_addr) {
    memset(&server_buffer, 0, BUFFER_SIZE);
  };

  void start();

  ~ClientManager();
};

#endif // CLIENTMANAGER_H
