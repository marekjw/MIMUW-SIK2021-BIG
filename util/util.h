//
// Created by marek on 19/08/2021.
//

#ifndef UTIL_H
#define UTIL_H

#include <arpa/inet.h>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <vector>

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

  /**
   * Checks if the name is valid
   * empty name is valid
   * @param name
   * @return true if the name is valid, false otherwise
   */
  static bool is_name_valid(std::string &name);

  static bool get_ip_str(const sockaddr_storage *from, char *s, int &port);

  template <typename T>
  static std::vector<unsigned char> &serialize(std::vector<unsigned char> &v,
                                               const T &obj);
};
#endif // UTIL_H
