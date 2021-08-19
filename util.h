//
// Created by marek on 19/08/2021.
//

#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
class util {
public:
  /**
   * Reads 4 bytes from ptr, converts them from network endian notation
   * to host endian notation
   * doesn't check if the memory is accessible
   * @param ptr
   * @return
   */
  static uint32_t read_uint32_from_network_stream(unsigned char *ptr);
};
#endif // UTIL_H
