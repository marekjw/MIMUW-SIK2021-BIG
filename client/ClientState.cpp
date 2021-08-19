#include "ClientState.h"
void ClientState::new_player(const std::string& new_player_name) {
  players_names.push_back(new_player_name);
}
void ClientState::game_over() {
  game = false;
  // TODO cos z numerem gierki
  players_names.clear(); // TODO czy na pewno?
}
void ClientState::update_direction(msg_from_gui event)  {

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
