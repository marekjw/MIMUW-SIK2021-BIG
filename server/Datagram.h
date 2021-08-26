#ifndef DATAGRAM_H
#define DATAGRAM_H

#include "../util/util.h"
#include <cstdint>
#include <string>

class Datagram {
private:
  uint32_t next_expected_event_no;
  uint64_t session_id;
  std::string name;
  short turn_direction;

public:
  Datagram(unsigned char *buffer, long len);

  const std::string &get_name() { return name; };

  [[nodiscard]] uint32_t get_next_expected_event_no() const {
    return next_expected_event_no;
  }

  bool invalid_name() { return !util::is_name_valid(name); };

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  [[nodiscard]] const std::string &get_name() const { return name; }

  [[nodiscard]] uint64_t get_session_id() const { return session_id; }
  bool invalid_crc();
};

#endif // DATAGRAM_H
