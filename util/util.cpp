//
// Created by marek on 19/08/2021.
//

#include "util.h"
#include <netinet/in.h>
#include <algorithm>

uint32_t util::read_uint32_from_network_stream(unsigned char *ptr) {
  return ntohl(*reinterpret_cast<uint32_t *>(ptr));
}

bool util::is_name_valid(std::string &name) {
  return name.size() <= 20 && std::all_of(name.begin(), name.end(), [](char c) {
    return 33 <= c && c <= 126;
  });
}