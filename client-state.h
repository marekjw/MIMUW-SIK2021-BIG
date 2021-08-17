

#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include <iostream>
#include <pthread.h>
#include <utility>

#define LEFT_DIRECTION 2
#define STRAIGHT 0
#define RIGHT_DIRECTION 1

class ClientState {
private:
  std::string name;
  short turn_direction;
  uint32_t next_expected_event_no;
  uint64_t session_id{};

public:
  explicit ClientState(std::string name, uint64_t session_id)
      : name(std::move(name)), turn_direction(0), next_expected_event_no(0),
        session_id(session_id){};

  void set_turn_direction(short turn) { turn_direction = turn; }

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  void set_next_expected_event_no(uint32_t no) { next_expected_event_no = no; }

  [[nodiscard]] uint32_t get_next_expected_event_no() const {
    return next_expected_event_no;
  }

  void set_session_id(int id) { session_id = id; }

  [[nodiscard]] int get_session_id() const { return session_id; }

  std::string &get_name() { return name; }
};

#endif
