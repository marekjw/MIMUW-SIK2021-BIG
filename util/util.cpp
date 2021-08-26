#include "util.h"
#include "err.h"
#include <algorithm>
#include <cstring>
#include <netinet/in.h>

uint32_t util::read_uint32_from_network_stream(unsigned char *ptr) {
  return ntohl(*reinterpret_cast<uint32_t *>(ptr));
}

bool util::is_name_valid(std::string &name) {
  return name.size() <= 20 && std::all_of(name.begin(), name.end(), [](char c) {
           return 33 <= c && c <= 126;
         });
}
bool util::get_ip_str(const sockaddr_storage *from, char *res, int &port) {
  switch (from->ss_family) {
  case AF_INET:
    // IP v4
    if (inet_ntop(AF_INET, &((struct sockaddr_in *)from)->sin_addr, res,
                  INET_ADDRSTRLEN) == nullptr) {
      warning("Could not parse IP4 address");
      return false;
    }
    port = ntohs(((struct sockaddr_in *)from)->sin_port);
    break;
  case AF_INET6:
    // IP v6
    if (inet_ntop(AF_INET6, &((struct sockaddr_in6 *)from)->sin6_addr, res,
                  INET6_ADDRSTRLEN) == nullptr) {
      warning("Could not parse IP6 address");
      return false;
    }
    port = ntohs(((struct sockaddr_in6 *)from)->sin6_flowinfo);
    break;
  default:
    warning("Unknown AF");
    return false;
  }
  return true;
}
template <typename T>
std::vector<unsigned char> &util::serialize(std::vector<unsigned char> &v,
                                            const T &obj) {
  auto size = v.size();
  v.resize(size + sizeof(T));

  std::memcpy(&v[size], &obj, sizeof(T));
  return v;
}
