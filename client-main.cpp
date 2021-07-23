
#include <cstdlib>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "client-state.h"

bool is_name_valid(std::string &name) {
  return std::all_of(name.begin(), name.end(),
                     [](char c) { return 33 <= c && c <= 126; });
}

static void client_to_gui() {}

static void gui_to_client() {}

static void server_to_client() {}

static void client_to_server() {}

int main(int argc, char **argv) {
  std::string game_address, name, gui_address = "localhost";

  int game_port = 2021, gui_port = 20210;

  int c;

  if (argc < 2) {
    std::cerr << "No game address provided" << std::endl;
    exit(1);
  }

  game_address = std::string(argv[1]);

  while ((c = getopt(argc, argv, "n:p:i:r:")) != -1) {
    switch (c) {
    case 'n':
      name = std::string(optarg);
      break;

    case 'p':
      game_port = atoi(optarg);
      break;

    case 'i':
      gui_address = std::string(optarg);
      break;

    case 'r':
      gui_port = atoi(optarg);
      break;
    default:
      std::cerr << "Invalid option" << std::endl;
      exit(1);
    }
  }

  if (!is_name_valid(name)) {
    std::cerr << "Invalid player name" << std::endl;
    exit(1);
  }

  printf("%s, %s, %s, %d, %d\n", game_address.data(), name.data(),
         gui_address.data(), game_port, gui_port);

  ClientState state{name};


}