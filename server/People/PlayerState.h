
#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "../../util/constants.h"
#include "../Datagram.h"
#include "Person.h"
#include <atomic>
#include <cmath>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>
#include <utility>

class PlayerState : public Person {
private:
  int turn_direction, direction;
  // accesed only in ServerManager game_loop
  double long x{}, y{};
  static int turning_speed;

  PlayerState **map_pointer{};

  int number;

  std::string name;
  bool alive, ready, connected;

  static double d2r(double d) { return (d / 180.0) * ((double)M_PI); }

public:
  static void initialize(int speed) { turning_speed = speed; }

  PlayerState(sockaddr_storage addr, const Datagram &datagram)
      : Person(addr, datagram.get_session_id()), ready(false), connected(true),
        name(datagram.get_name()), alive(true),
        turn_direction(datagram.get_turn_direction()), number(0) {
    std::cerr << "New player: " << name << "\n";
  }

  inline void set_turn_direction(int new_turn_direction) {
    if (new_turn_direction == STRAIGHT ||
        new_turn_direction == LEFT_DIRECTION ||
        new_turn_direction == RIGHT_DIRECTION) {
      turn_direction = new_turn_direction;
    }
  }

  void update_direction();

  /**
   * Updates the player position
   * @returns true if player changed pixel, false otherwise
   */
  bool move();

  [[nodiscard]] int get_number() const { return number; }

  std::pair<int, int> get_position();

  void kill() { alive = false; }

  void set_number(int id) { number = id; }

  void make_ready() {
    std::cerr << "Player " << name << " ready!\n";
    ready = true;
  };

  [[nodiscard]] bool is_ready() const { return ready; }
  [[nodiscard]] bool is_alive() const;

  void disconnect() { connected = false; }

  [[nodiscard]] PlayerState **get_map_pointer() const { return map_pointer; }

  void set_map_pointer(PlayerState **ptr) { map_pointer = ptr; }

  [[nodiscard]] const std::string &get_name() const { return name; }

  void set_position(double new_x, double new_y, int new_direction) {
    x = new_x;
    y = new_y;
    direction = new_direction;
  }

  [[nodiscard]] int get_x() const { return x; }
  [[nodiscard]] int get_y() const { return y; }

  [[nodiscard]] bool is_connected() const { return connected; }
};

#endif
