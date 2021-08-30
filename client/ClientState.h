

#ifndef CLIENT_STATE_H
#define CLIENT_STATE_H

#include "../util/constants.h"
#include "Event.h"
#include <atomic>
#include <condition_variable>
#include <endian.h>
#include <iostream>
#include <map>
#include <mutex>
#include <pthread.h>
#include <set>
#include <utility>
#include <vector>

class ClientState {
private:
  std::string name;
  std::atomic<short> turn_direction;
  uint32_t next_expected_event_no; // number of the next expected event to be
                                   // received from the server
  uint32_t next_event_to_send_no; // number of the next event to send to gui, is
                                  // used by multiple threads
  uint64_t session_id{}, n_session_id;

  bool game_no_set;
  uint32_t game_no;

  int gui_socket;

  std::set<uint32_t> used_game_ids;

  bool left_key_pressed, right_key_pressed, game = true;

  std::vector<std::string> players_names;

  // events queue
  std::map<uint32_t, Event> queue;

  // thread safety
  std::mutex queue_mutex;
  std::condition_variable cv;

  std::mutex player_vector_mutex;
  std::mutex event_no_mutex;

public:
  explicit ClientState(std::string name, uint64_t session_id, int gui_socket)
      : name(std::move(name)), turn_direction(0), next_expected_event_no(0),
        next_event_to_send_no(0), session_id(session_id), game_no(0),
        gui_socket(gui_socket), game_no_set(false), left_key_pressed(false),
        right_key_pressed(false), n_session_id(htobe64(session_id)){};

  [[nodiscard]] short get_turn_direction() const { return turn_direction; }

  void set_next_expected_event_no(uint32_t no) {
    event_no_mutex.lock();
    next_expected_event_no = std::max(no, next_expected_event_no);
    event_no_mutex.unlock();
  }

  [[nodiscard]] uint32_t get_next_expected_event_no() {
    event_no_mutex.lock();
    uint32_t res = next_expected_event_no;
    event_no_mutex.unlock();
    return res;
  }

  void reset();

  std::string &get_name() { return name; }

  game_number_validity valid_game_number(uint32_t game_number);

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
  [[noreturn]] void dispatch_queue();

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
  std::string get_player_name(size_t index);

  /**
   * Appends all the player names to the res string
   *
   * is thread safe
   * @param res
   */
  void append_player_names(std::string &res);

  [[nodiscard]] uint64_t get_n_session_id() const { return n_session_id; }

  /**
   * sends an event to gui, using the socket
   * the socket has to be connected
   * @param event - an event to be sent
   * @param socket - socket over which an event is to be sent
   */
  void send_event_to_gui(Event event) const;
  void finish_sending_events();
};

// TODO remember to delete strings, cuz they're only pointers rn
#endif
