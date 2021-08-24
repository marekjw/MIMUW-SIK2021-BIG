#include "GameState.h"
void GameState::set_up_new_game() {
  // TODO
}

bool GameState::is_game_on() {
  // TODO make thread safe
  return false;
}

bool GameState::pixel_valid(std::pair<int, int> position) {
  if (position.first < 0 || position.second < 0)
    return false;

  if (pixels.size() <= position.first || pixels[0].size() <= position.second)
    return false;

  return pixels[position.first][position.second];
}

void GameState::eat_pixel(std::pair<int, int> pixel) {
  pixels[pixel.first][pixel.second] = false;
}
bool GameState::update_spectator(const Datagram &datagram,
                                 const sockaddr_in &address) {

  auto it = spectators.find({
      datagram.get_session_id(),
      ntohs(address.sin_port),
  });
  return false;
}
