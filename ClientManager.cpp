
#include "ClientManager.h"
#include "client-state.h"

#include <thread>


void ClientManager::start() {
  std::thread threads[4];

  threads[0] = std::thread(&ClientManager::client_to_gui, this);
  threads[1] = std::thread(&ClientManager::gui_to_client, this);
  threads[2] = std::thread(&ClientManager::client_to_server, this);
  threads[3] = std::thread(&ClientManager::server_to_client, this);

  for(auto &t : threads) t.join();
}

[[noreturn]] void ClientManager::client_to_gui() {
  // TODO
  while(true){

  }
}

void ClientManager::gui_to_client() {
  // TODO
}

void ClientManager::client_to_server() {
  // TODO
}

void ClientManager::server_to_client() {
  // TODO
}
