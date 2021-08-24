#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Datagram.h"
#include "People/PLayerState.h"
#include "People/Person.h"
#include <map>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

class GameState {
private:
  int width, height, turning_speed, round_per_sec;
  std::vector<PLayerState> players_sorted;
  uint32_t game_id;

  std::map<std::pair<uint64_t, int>, PLayerState *> players;

  std::map<std::pair<uint64_t, int>, Person> spectators;

  std::mutex map_mutex;

public:
  GameState(int width, int height, int turning_speed, int rounds_per_sec)
      : width(width), height(height), turning_speed(turning_speed),
        round_per_sec(rounds_per_sec) {
    PLayerState::initialize(turning_speed);
  }

  [[nodiscard]] int get_rounds_per_sec() const { return round_per_sec; }

  /**
   * Returns the number of ready players_sorted
   * - that is players_sorted, who sent turn_direction /= 0 at least once
   * @return
   */
  int ready_players();

  void set_up_new_game();
  /**
   * Is not thread safe
   * @return reference to the players_sorted vector
   */
  std::vector<PLayerState> &get_players() { return players_sorted; }

  bool pixel_valid(std::pair<int, int> position);

  /**
   * The given pixel becomes invalid
   * @param pixel
   */
  void eat_pixel(std::pair<int, int> pixel);

  /**
   * @return number of alive players_sorted
   */
  int players_alive() const;

  bool is_game_on();

  /**
   * Self explanatory
   */
  [[nodiscard]] uint32_t get_game_id() const { return game_id; }

  /**
   * Thread safe
   * @param datagram
   * @param address
   */
  void add_spectator(const Datagram &datagram, const sockaddr_in &address);

  /**
   * Thread safe
   * @param datagram
   * @param address
   */
  void update_player(const Datagram &datagram, const sockaddr_in &address);

  void kill_player(PLayerState &player);

  std::mutex &get_map_mutex() { return map_mutex; }

  const std::map<std::pair<uint64_t, int>, PLayerState *> &get_players_map() {
    return players;
  };

  const std::map<std::pair<uint64_t, int>, Person> &
  get_spectators_map() {
    return spectators;
  }

  void reset();

  void disconnect_inactive_ones();
};

#endif // GAMESTATE_H
