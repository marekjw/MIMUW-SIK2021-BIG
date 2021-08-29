#include "PlayerState.h"

int PlayerState::turning_speed;

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

/**
 * @return (x,y) - player's position
 */
std::pair<int, int> PlayerState::get_position() {
  return std::pair<int, int>(x, y);
}
bool PlayerState::is_alive() const { return alive; }

/**
 * Moves the player by one unit
 * @return true if player changed pixel
 */
bool PlayerState::move() {
  int old_x = x, old_y = y;

  x += cos(d2r(direction));
  y += sin(d2r(direction));

  return !((int)x == old_x && (int)y == old_y);
}
