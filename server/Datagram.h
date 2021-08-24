//
// Created by marek on 23/08/2021.
//

#ifndef BIG_ZADANIE_DATAGRAM_H
#define BIG_ZADANIE_DATAGRAM_H

#include <cstdint>
#include <string>

class Datagram {
private:
  uint32_t next_expected_event_no;

public:
  Datagram(unsigned char *buffer, long len);

  const std::string &get_name();

  [[nodiscard]] uint32_t get_next_expected_event_no() const { return next_expected_event_no; }

  bool invalid_name();
};

#endif // BIG_ZADANIE_DATAGRAM_H
