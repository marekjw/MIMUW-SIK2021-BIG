#include "ClientState.h"
void ClientState::new_player(const std::string& new_player_name) {
  players_names.push_back(new_player_name);
}
void ClientState::game_over() {
  game = false;
  // TODO cos z numerem gierki
  players_names.clear(); // TODO czy na pewno?
}
