#include <cstdlib>
#include <unistd.h>

#include <ctime>

#include <iostream>

int main(int argc, char **argv) {
  int port_no = 2021, seed = time(NULL), turning_speed = 6, round_per_sec = 50,
      width = 640, height = 480;

  int c;

  while ((c = getopt(argc, argv, "p:s:t:v:w:h:")) != -1) {
    switch (c) {
    case 'p':
      port_no = atoi(optarg);
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
      std::cerr << "Invalid option" << std::endl;
      exit(1);
    }
  }
}
