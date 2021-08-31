#include "../util/err.h"
#include "GameState.h"
#include "ServerManager.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int seed = time(NULL);
  int turning_speed = 6, round_per_sec = 50, width = 640,
      height = 480;

  std::string port_no{"2021"};

  int c;

  while ((c = getopt(argc, argv, "p:s:t:v:w:h:")) != -1) {
    switch (c) {
    case 'p':
      port_no = std::string(optarg);
      break;
    case 's':
      seed = atoi(optarg);
      break;
    case 't':
      turning_speed = atoi(optarg);
      break;
    case 'v':
      round_per_sec = atoi(optarg);
      break;
    case 'w':
      width = atoi(optarg);
      break;
    case 'h':
      height = atoi(optarg);
      break;
    default:
      fatal("Invalid option");
    }
  }

  addrinfo *addr, hints;
  memset(&hints, 0, sizeof hints);
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_INET6;

  int server_socket, status;
  if ((status = getaddrinfo(nullptr, port_no.data(), &hints, &addr)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  server_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
  if (bind(server_socket, addr->ai_addr, addr->ai_addrlen) != 0) {
    syserr("bind error");
  }

  Random rand{seed};

  GameState game{width, height, turning_speed, rand};

  ServerManager server{game, server_socket, (float)round_per_sec};

  server.start();
}
