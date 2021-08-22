#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "../util/constants.h"
#include "Events/Event.h"
#include "GameState.h"
#include <condition_variable>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <vector>

class ServerManager {
private:
  int sockfd;

  char buffer[BUFFER_SIZE];

  GameState &state;


  // events management
  std::mutex event_mutex;
  std::condition_variable cv;
  std::vector<Event> events;

  uint32_t next_event_to_send;

  /**
   * Runs in an endless loop
   * Checks if a new game can be started, and starts one if it can
   */
  [[noreturn]] void main_loop();

  void game_loop();

  /**
   * Thread worker that sends new events to all the clients
   * In other words, dispathes the events queue
   */
  [[noreturn]] void send_events_queue();

  /**
   * Listens for incoming datagrams.
   * Updates players data
   */
  [[noreturn]] void listener();


  void add_event(Event event);

  uint32_t round_time; // round time in miliseconds

  void send_event_to_all(Event *event_pointer);

public:
  ServerManager(GameState &gameState, int sockfd)
      : sockfd(sockfd), state(gameState), next_event_to_send(0) {}

  /**
   * Start the server loop.
   * Reads players data.
   * Updates events.
   */
  void start();

};

#endif // SERVERMANAGER_H
