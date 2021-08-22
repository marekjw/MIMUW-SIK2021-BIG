//
// Created by marek on 22/08/2021.
//

#include "PLayerState.h"
void PLayerState::update_direction() {
  switch (turn_direction) {
  case LEFT_DIRECTION:
    direction -= turning_speed;
    break;
  case RIGHT_DIRECTION:
    direction += turning_speed;
    break;
  }
}
std::pair<int, int> PLayerState::get_position() {
  return std::pair<int, int>(x ,y);
}
