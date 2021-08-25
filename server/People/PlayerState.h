
#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "../../util/constants.h"
#include "../Datagram.h"
#include "Person.h"
#include <atomic>
#include <map>
#include <netinet/in.h>
#include <string>
#include <utility>

// TODO double precision

class PlayerState : public Person {
private:
  std::atomic<int> turn_direction, direction;
  // accesed only in ServerManager game_loop
  double long x, y;
  static int turning_speed;

  PlayerState **map_pointer;

  const int number;

  std::string name;
  bool alive;

public:
  static void initialize(int speed) { turning_speed = speed; }

  // TODO finish the constructor
  PlayerState(sockaddr_storage addr, int number, const Datagram &datagram)
      : Person(addr, datagram.get_session_id()), number(number),
        name(datagram.get_name()),
        turn_direction(datagram.get_turn_direction()) {}

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

  void kill(){ alive = false;}

  void make_ready();

  bool is_ready();
  bool is_alive() const;

  void disconnect();

  [[nodiscard]] PlayerState **get_map_pointer() const { return map_pointer; }

  void set_map_pointer(PlayerState **ptr) { map_pointer = ptr; }

  [[nodiscard]] const std::string &get_name() const { return name; }
};

#endif
