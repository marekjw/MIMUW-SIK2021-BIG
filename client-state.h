

#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include <iostream>
#include <pthread.h>
#include <utility>

class ClientState {
private:
  std::string name;
  short turn_direction;
  uint32_t next_expected_event_no;

public:
  explicit ClientState(std::string name)
      : name(std::move(name)), turn_direction(0), next_expected_event_no(0){};

  void set_turn_direction(short turn) { turn_direction = turn; }

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  void set_next_expected_event_no(uint32_t no) { next_expected_event_no = no; }

  [[nodiscard]] uint32_t get_next_expected_event_no() const {
    return next_expected_event_no;
  }
};

#endif
