//
// Created by marek on 19/08/2021.
//

#include "util.h"
#include <netinet/in.h>

uint32_t util::read_uint32_from_network_stream(unsigned char *ptr) {
  return ntohl(*reinterpret_cast<uint32_t *>(ptr));
}
