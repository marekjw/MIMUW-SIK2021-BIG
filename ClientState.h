

#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "constants.h"
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <utility>
#include <vector>

class ClientState {
private:
  std::string name;
  short turn_direction;
  uint32_t next_expected_event_no; // number of the next expected event to be
                                   // received from the server
  uint32_t next_event_to_send_no;  // number of the next event to send to gui
  uint64_t session_id{};

  bool game_no_set;
  uint32_t game_no;

  bool left_key_pressed, right_key_pressed, game = true;

  std::vector<std::string>players_names;

public:
  explicit ClientState(std::string name, uint64_t session_id)
      : name(std::move(name)), turn_direction(0), next_expected_event_no(0),
        next_event_to_send_no(0), session_id(session_id), game_no(0),
        game_no_set(false), left_key_pressed(false), right_key_pressed(false){};

  void set_turn_direction(short turn) { turn_direction = turn; }

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  void set_next_expected_event_no(uint32_t no) { next_expected_event_no = no; }

  [[nodiscard]] uint32_t get_next_expected_event_no() const {
    return next_expected_event_no;
  }

  void set_session_id(int id) { session_id = id; }

  [[nodiscard]] uint64_t get_session_id() const { return session_id; }

  std::string &get_name() { return name; }

  bool valid_game_number(uint32_t game_number) {
    if (!game_no_set) {
      game_no = game_number;
      game_no_set = true;
      return true;
    }

    if (game_number > game_no) {
      game_no = game_number;
      return true;
    }

    return false;
  }

  [[nodiscard]] bool is_playing() const { return game; }
  void game_over();
  void play_game() { game = true; }

  void update_direction(msg_from_gui event) {

    switch (event) {
    case LEFT_KEY_DOWN:
      left_key_pressed = true;
      turn_direction = LEFT_DIRECTION;
      break;
    case RIGHT_KEY_DOWN:
      right_key_pressed = true;
      turn_direction = RIGHT_DIRECTION;
      break;
    case LEFT_KEY_UP:
      left_key_pressed = false;
      turn_direction = right_key_pressed ? RIGHT_DIRECTION : STRAIGHT;
      break;
    case RIGHT_KEY_UP:
      right_key_pressed = false;
      turn_direction = left_key_pressed ? LEFT_DIRECTION : STRAIGHT;
      break;
    case UNKNOWN:
      break;
    }
  }

  void new_player(const std::string& new_player_name);
};

#endif
