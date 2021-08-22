#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "PLayerState.h"
#include <vector>

class GameState {
private:
  int width, height, turning_speed, round_per_sec;
  std::vector<PLayerState> players;

public:
  GameState(int width, int height, int turning_speed, int rounds_per_sec)
      : width(width), height(height), turning_speed(turning_speed),
        round_per_sec(rounds_per_sec) {
    PLayerState::initialize(turning_speed);
  }

  [[nodiscard]] int get_rounds_per_sec() const { return round_per_sec; }

  /**
   * Returns the number of ready players
   * - that is players, who sent turn_direction /= 0 at least once
   * @return
   */
  int ready_players();

  void set_up_new_game();
  /**
   * Is not thread safe
   * @return reference to the players vector
   */
  std::vector<PLayerState> &get_players() { return players; }

  bool pixel_valid(std::pair<int, int> position);

  /**
   * @return number of alive players
   */
  int players_alive();

  bool is_game_on();
};

#endif // GAMESTATE_H
