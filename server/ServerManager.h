#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "../util/constants.h"
#include "Event.h"
#include "GameState.h"
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <vector>

class ServerManager {
private:
  int sockfd;
  GameState &state;

  std::vector<Event> events;

  uint32_t next_event_to_send;

public:
  ServerManager(GameState &gameState, int sockfd)
      : sockfd(sockfd), state(gameState), next_event_to_send(0) {}

  /**
   * Start the server loop.
   * Reads players data.
   * Updates events.
   */
  void start();

  /**
   * Runs in an endless loop
   */
  void main_game_loop();

  /**
   * Thread worker that sends new events to all the clients
   */
  void send_events_queue();

  /**
   * Listens for incoming datagrams.
   * Updates players data
   */
  void listener();
};

#endif // SERVERMANAGER_H
