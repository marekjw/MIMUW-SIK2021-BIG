

#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "../util/constants.h"
#include "Event.h"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <map>
#include <mutex>
#include <pthread.h>
#include <utility>
#include <vector>

class ClientState {
private:
  std::string name;
  std::atomic<short> turn_direction;
  uint32_t next_expected_event_no; // number of the next expected event to be
                                   // received from the server
  uint32_t next_event_to_send_no;  // number of the next event to send to gui
  uint64_t session_id{};

  bool game_no_set;
  uint32_t game_no;

  bool left_key_pressed, right_key_pressed, game = true;

  std::vector<std::string> players_names;

  // events queue
  std::map<uint32_t, Event> queue;

  // thread safety
  std::mutex queue_mutex;
  std::condition_variable cv;

public:
  explicit ClientState(std::string name, uint64_t session_id)
      : name(std::move(name)), turn_direction(0), next_expected_event_no(0),
        next_event_to_send_no(0), session_id(session_id), game_no(0),
        game_no_set(false), left_key_pressed(false), right_key_pressed(false){};

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  void set_next_expected_event_no(uint32_t no) { next_expected_event_no = no; }

  [[nodiscard]] uint32_t get_next_expected_event_no() const {
    return next_expected_event_no;
  }

  [[nodiscard]] uint64_t get_session_id() const { return session_id; }

  std::string &get_name() { return name; }

  bool valid_game_number(uint32_t game_number);

  [[nodiscard]] bool is_playing() const { return game; }
  void game_over();
  void play_game() { game = true; }

  void update_direction(msg_from_gui event);

  /**
   * Adds new player name at the end of the players_sorted list
   * Thread safe
   * @param new_player_name
   */
  void new_player(const std::string &new_player_name);

  /**
   * Starts sending messages from the queue to the specified socket
   * @param socket
   */
  [[noreturn]] void dispatch_queue(int socket);

  /**
   * Adds event to the event queue
   * is thread safe
   * @param event
   */
  void add_event(uint32_t event_no, Event event);

  /**
   * is thread safe
   * @param index
   * @return the name of the index-th player
   */
  const std::string &get_player_name(size_t index);

  /**
   * Appends all the player names to the res string
   *
   * is thread safe
   * @param res
   */
  void append_player_names(std::string &res);

  /**
   * sends an event to gui, using the socket
   * the socket has to be connected
   * @param event - an event to be sent
   * @param socket - socket over which an event is to be sent
   */
  static void send_event_to_gui(Event event, int socket);
};

// TODO remember to delete strings, cuz they're only pointers rn
#endif
