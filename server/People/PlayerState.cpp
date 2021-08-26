#include "PlayerState.h"

void PlayerState::update_direction() {
  if (!connected)
    return;

  switch (turn_direction) {
  case LEFT_DIRECTION:
    direction -= turning_speed;
    break;
  case RIGHT_DIRECTION:
    direction += turning_speed;
    break;
  }

  direction = direction % 360;
}
std::pair<int, int> PlayerState::get_position() {
  return std::pair<int, int>(x, y);
}
bool PlayerState::is_alive() const { return alive; }
bool PlayerState::move() {

  if (direction == 0) {
    y += 1;
    return true;
  }

  if (direction == 180) {
    y -= 1;
    return true;
  }

  if (direction == 90) {
    x += 1;
    return true;
  }

  if (direction == 270) {
    x -= 1;
    return true;
  }

  int old_x = x, old_y = y;

  x += tan(d2r(direction));
  y += tan(M_PI_2 - d2r(direction));

  return !((int)x == old_x && (int)y == old_y);
}
