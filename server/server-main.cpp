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

uint32_t parse_uint(char *string) {
  char *endptr;
  uint32_t res = strtoul(string, &endptr, 10);
  if (errno != 0) {
    syserr("Cannot parse number");
  }

  if (*endptr != ' ' && *endptr != 0) {
    fatal("Cannot parse number");
  }
  return res;
}

int main(int argc, char **argv) {
  auto seed = (uint32_t)time(nullptr);
  uint32_t round_per_sec = 50, width = 640, height = 480;
  int turning_speed = 6;

  std::string port_no{"2021"};

  int c;

  int options_count = 0;

  while ((c = getopt(argc, argv, "p:s:t:v:w:h:")) != -1) {
    ++options_count;
    switch (c) {
    case 'p':
      port_no = std::to_string(parse_uint(optarg));
      break;
    case 's':
      seed = parse_uint(optarg);
      break;
    case 't':
      turning_speed = (int)parse_uint(optarg);
      break;
    case 'v':
      round_per_sec = parse_uint(optarg);
      break;
    case 'w':
      width = parse_uint(optarg);
      break;
    case 'h':
      height = parse_uint(optarg);
      break;
    default:
      fatal("Invalid option");
    }
  }

  // check if values make sense
  if (seed == 0) {
    fatal("Seed cannot be equal to 0");
  }

  if (turning_speed <= 0 || 360 < turning_speed) {
    fatal("Invalid turning speed");
  }

  if (width == 0 || height == 0) {
    fatal("Invalid dimensions");
  }

  if (5000 < width || 5000 < height) {
    fatal("Invalid dimensions");
  }

  if (round_per_sec <= 0 || 1000 < round_per_sec) {
    fatal("Invalid rounds per second");
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
