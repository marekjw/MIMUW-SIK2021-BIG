#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../util/Random.h"
#include "Datagram.h"
#include "People/Person.h"
#include "People/PlayerState.h"
#include <map>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

// TODO - musi działać przenoszenie graczy z poprzedniej sesji (albo i nie xd)

class GameState {
private:
  std::vector<std::vector<bool>> pixels;

  uint32_t max_x, max_y;
  unsigned long ready_players_no, players_alive_no;
  std::vector<PlayerState> players_sorted;
  uint32_t game_id{};

  bool the_game_is_on;

  std::map<std::pair<std::string, int>, PlayerState *> players;

  std::map<std::pair<std::string, int>, Person> spectators;

  std::mutex clients_mutex, game_on_mutex;

  Random &rng;

  bool update_spectator_(const Datagram &datagram, const sockaddr_storage *from,  char *res, int port);

  bool update_player_(const Datagram &datagram, const sockaddr_storage *address);

public:
  GameState(int width, int height, int turning_speed, Random &random)
      : max_x(width), max_y(height), ready_players_no(0),
        the_game_is_on(false), players_alive_no(0), rng(random) {
    PlayerState::initialize(turning_speed);
    pixels =
        std::vector<std::vector<bool>>(width, std::vector<bool>(height, true));
  }

  /**
   * Returns the number of ready players_sorted
   * - that is players_sorted, who sent turn_direction /= 0 at least once
   * (thread safe)
   * @return
   */
  unsigned long ready_players();

  void set_up_new_game();
  /**
   * Is not thread safe
   * @return reference to the players_sorted vector
   */
  std::vector<PlayerState> &get_players() { return players_sorted; }

  bool pixel_valid(std::pair<int, int> position);

  /**
   * The given pixel becomes invalid
   * @param pixel - a valid pixel
   */
  void eat_pixel(std::pair<int, int> pixel);

  /**
   * @return number of alive players_sorted
   */
  [[nodiscard]] unsigned long players_alive() const;

  bool is_game_on();

  /**
   * Self explanatory
   */
  [[nodiscard]] uint32_t get_game_id() const { return game_id; }

  /**
   * Thread safe
   * @param datagram
   * @param from - is updated to point at the most current one
   * @return true if the spectator should not be ignored, false otherwise
   */
  bool update_spectator(const Datagram &datagram, const sockaddr_storage *from);

  /**
   * Thread safe
   * @param datagram
   * @param address
   * @return true if the player should not be ignored, false otherwise
   */
  bool update_player(const Datagram &datagram, const sockaddr_storage *address);

  void kill_player(PlayerState &player);

  std::mutex &get_map_mutex() { return clients_mutex; }

  const std::map<std::pair<std::string, int>, PlayerState *> &
  get_players_map() {
    return players;
  };

  const std::map<std::pair<std::string, int>, Person> &get_spectators_map() {
    return spectators;
  }

  void reset();

  void disconnect_inactive_ones();
  bool can_start_game();
  [[nodiscard]] uint32_t get_max_x() const;
  [[nodiscard]] uint32_t get_max_y() const;
};

#endif // GAMESTATE_H
