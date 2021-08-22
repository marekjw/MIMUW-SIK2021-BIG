
#ifndef BIG_ZADANIE_PLAYERSTATE_H
#define BIG_ZADANIE_PLAYERSTATE_H

#include "../util/constants.h"
#include <atomic>
#include <utility>

class PLayerState {
private:
  std::atomic<int> turn_direction, direction;
  // accesed only in ServerManager game_loop
  int x, y;
  static int turning_speed;

  const int number;

public:
  static void initialize(int speed) { turning_speed = speed; }

  // TODO finish the constructor
  PLayerState(int number) : number(number) {}

  void set_turn_direction(int new_turn_direction) {
    turn_direction = new_turn_direction;
  }

  void update_direction();

  int get_direction() { return direction; }

  /**
   * Updates the player position
   * @returns true if player changed pixel, false otherwise
   */
  bool move();

  [[nodiscard]] int get_number() const { return number; }

  std::pair<int, int> get_position();
};

#endif
