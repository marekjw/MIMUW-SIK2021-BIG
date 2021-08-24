
#ifndef BIG_ZADANIE_PLAYERSTATE_H
#define BIG_ZADANIE_PLAYERSTATE_H

#include "../util/constants.h"
#include <atomic>
#include <netinet/in.h>
#include <utility>

class PLayerState {
private:
  std::atomic<int> turn_direction, direction;
  // accesed only in ServerManager game_loop
  int x, y;
  static int turning_speed;

  sockaddr_in address;

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

  const sockaddr_in &get_address() { return address; }

  void kill();

  uint64_t get_last_activity_timestamp();
};

#endif
