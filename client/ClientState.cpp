#include "ClientState.h"
#include "../util/err.h"
#include <netinet/in.h>
#include <sys/socket.h>

void ClientState::new_player(const std::string &new_player_name) {
  players_names.push_back(new_player_name);
}

void ClientState::game_over() {
  game = false;
  // TODO cos z numerem gierki
  // TODO numery expectd: send i ten drugi
  players_names.clear(); // TODO czy na pewno?
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
[[noreturn]] void ClientState::dispatch_queue(int socket) {
  std::unique_lock<std::mutex> lock(queue_mutex);

  do {
    cv.wait(lock, [this] { return !queue.empty(); });

    if (!queue.empty()) {
      auto event_record = *queue.begin();
      queue.erase(queue.begin());
      next_event_to_send_no = event_record.first + 1;

      lock.unlock();

      send_event_to_gui(event_record.second, socket);

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

const std::string &ClientState::get_player_name(size_t index) {
  if (index >= players_names.size())
    fatal("Data makes no sense - player number out of bonds");
  return players_names[index];
}

void ClientState::append_player_names(std::string &res) {
  for (auto const &n : players_names) {
    res.push_back(' ');
    res.append(n);
  }
}

bool ClientState::valid_game_number(uint32_t game_number) {
  // TODO
  return true;
}
void ClientState::send_event_to_gui(Event event, int socket) {
  if (event.get_type() == GAME_OVER_EVENT)
    return;
  std::cerr << "SENDING TO GUI: " << *event.get_data_ptr() << "\n";
  send(socket, event.get_data_ptr()->data(), event.get_data_ptr()->size(), 0);
}
