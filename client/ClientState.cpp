#include "ClientState.h"
#include "../util/err.h"
#include <netinet/in.h>
#include <sys/socket.h>

void ClientState::new_player(const std::string &new_player_name) {
  player_vector_mutex.lock();
  players_names.push_back(new_player_name);
  player_vector_mutex.unlock();
}

void ClientState::update_direction(msg_from_gui event) {

  switch (event) {
  case LEFT_KEY_DOWN:
    left_key_pressed = true;
    turn_direction = LEFT_DIRECTION;
    break;
  case RIGHT_KEY_DOWN:
    right_key_pressed = true;
    turn_direction = RIGHT_DIRECTION;
    break;
  case LEFT_KEY_UP:
    left_key_pressed = false;
    turn_direction = right_key_pressed ? RIGHT_DIRECTION : STRAIGHT;
    break;
  case RIGHT_KEY_UP:
    right_key_pressed = false;
    turn_direction = left_key_pressed ? LEFT_DIRECTION : STRAIGHT;
    break;
  case UNKNOWN:
    break;
  }
}
[[noreturn]] void ClientState::dispatch_queue() {
  std::unique_lock<std::mutex> lock(queue_mutex);

  do {
    cv.wait(lock, [this] { return !queue.empty(); });

    if (!queue.empty()) {
      auto event_record = *queue.begin();
      queue.erase(queue.begin());
      next_event_to_send_no = event_record.first + 1;

      lock.unlock();

      send_event_to_gui(event_record.second);

      // free the memory
      delete event_record.second.get_data_ptr();

      lock.lock();
    }
  } while (true);
}

void ClientState::add_event(uint32_t event_no, Event event) {
  std::unique_lock<std::mutex> lock(queue_mutex);

  if (event_no >= next_event_to_send_no) {
    queue.insert({event_no, event});
  }

  lock.unlock();
  cv.notify_one();
}

std::string ClientState::get_player_name(size_t index) {
  player_vector_mutex.lock();
  if (index >= players_names.size())
    fatal("Data makes no sense - player number out of bonds");
  std::string res = players_names[index];
  player_vector_mutex.unlock();
  return res;
}

void ClientState::append_player_names(std::string &res) {
  player_vector_mutex.lock();
  for (auto const &n : players_names) {
    res.push_back(' ');
    res.append(n);
  }
  player_vector_mutex.unlock();
}

game_number_validity ClientState::valid_game_number(uint32_t game_number) {
  if (!game_no_set) {
    game_no_set = true;
    game_no = game_number;
    return VALID;
  }

  if (game_no == game_number) {
    return VALID;
  }

  if (used_game_ids.find(game_number) == used_game_ids.end()) {
    used_game_ids.insert(game_no);
    game_no = game_number;
    return VALID_NEW_GAME;
  }

  return INVALID;
}
void ClientState::send_event_to_gui(Event event) const {
  if (event.get_type() == GAME_OVER_EVENT)
    return;
  send(gui_socket, event.get_data_ptr()->data(), event.get_data_ptr()->size(), 0);
}
void ClientState::reset() {
  event_no_mutex.lock();
  queue_mutex.lock();
  player_vector_mutex.lock();

  finish_sending_events();
  next_event_to_send_no = 0;

  players_names.clear();

  for (auto it : queue) {
    delete it.second.get_data_ptr();
  }
  queue.clear();

  next_expected_event_no = 0;

  player_vector_mutex.unlock();
  queue_mutex.unlock();
  event_no_mutex.unlock();
}

void ClientState::finish_sending_events() {
  while (!queue.empty()){
    auto event_record = *queue.begin();
    queue.erase(queue.begin());
    send_event_to_gui(event_record.second);
  }
}
