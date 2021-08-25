#include "PlayerState.h"
void PlayerState::update_direction() {
  switch (turn_direction) {
  case LEFT_DIRECTION:
    direction -= turning_speed;
    break;
  case RIGHT_DIRECTION:
    direction += turning_speed;
    break;
  }
}
std::pair<int, int> PlayerState::get_position() {
  return std::pair<int, int>(x ,y);
}
bool PlayerState::is_alive() const { return alive;}
