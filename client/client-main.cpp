
#include "../util/err.h"
#include "../util/util.h"
#include "ClientManager.h"
#include "ClientState.h"
#include "StreamBuffer.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

static int setup_connection(const char *address, const char *port,
                            const addrinfo *hints, addrinfo *&addr_ptr) {
  int sock, status;
  if ((status = getaddrinfo(address, port, hints, &addr_ptr)) < 0) {
    fprintf(stderr, "getaddrinfo (%s) error: %s\n", address,
            gai_strerror(status));
    exit(1);
  }

  sock =
      socket(addr_ptr->ai_family, addr_ptr->ai_socktype, addr_ptr->ai_protocol);

  if (sock < 0)
    syserr("Could not create socket");
  if (hints->ai_protocol != IPPROTO_UDP) {
    if (connect(sock, addr_ptr->ai_addr, addr_ptr->ai_addrlen) < 0)
      syserr("Could not connect socket");
  }
  return sock;
}

int main(int argc, char **argv) {
  // session id is the current timestamp in microseconds
  uint64_t session_id = std::chrono::duration_cast<std::chrono::microseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();

  printf("%ld\n", session_id);
  std::string game_address, name, gui_address = "localhost";

  std::string game_port{"2021"}, gui_port{"20210"};

  int c;

  if (argc < 2) {
    fatal("No game address provided");
  }

  game_address = std::string(argv[1]);

  while ((c = getopt(argc, argv, "n:p:i:r:")) != -1) {
    switch (c) {
    case 'n':
      name = std::string(optarg);
      break;

    case 'p':
      game_port = std::string(optarg);
      break;

    case 'i':
      gui_address = std::string(optarg);
      break;

    case 'r':
      gui_port = std::string(optarg);
      break;
    default:
      fatal("Invalid option");
    }
  }

  if (!util::is_name_valid(name)) {
    fatal("Invalid name");
  }

  printf("%s, %s, %s, %s, %s\n", game_address.data(), name.data(),
         gui_address.data(), game_port.data(), gui_port.data());

  addrinfo *game_addrinfo, *gui_addrinfo, hints;

  memset(&hints, 0, sizeof hints);
  int game_socket, gui_socket;

  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_socktype = SOCK_DGRAM;
  game_socket = setup_connection(game_address.data(), game_port.data(), &hints,
                                 game_addrinfo);

  std::cout << "GAME OK\n";

  memset(&hints, 0, sizeof hints);
  hints.ai_protocol = IPPROTO_TCP;
  gui_socket = setup_connection(gui_address.data(), gui_port.data(), &hints,
                                gui_addrinfo);
  ClientState state{name, session_id, gui_socket};

  StreamBuffer gui_buffer{gui_socket};

  ClientManager client{state, gui_buffer, game_socket, gui_socket,
                       game_addrinfo};

  std::cout << "Everything ok: " << game_socket << " gui:" << gui_socket
            << "\n";

  client.start();
}