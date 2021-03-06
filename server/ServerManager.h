#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include "../util/constants.h"
#include "Events/Event.h"
#include "GameState.h"
#include <cstdio>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <vector>

class ServerManager {
private:
  int sockfd;

  unsigned char buffer[BUFFER_SIZE]{};

  GameState &state;

  // events management
  std::mutex event_mutex;
  std::vector<Event> events;

  uint32_t next_event_to_send;

  /**
   * Runs in an endless loop
   * Checks if a new game can be started, and starts one if it can
   */
  [[noreturn]] void main_loop();

  void game_loop();

  /**
   * Listens for incoming datagrams.
   * Updates players_sorted data
   * Only reads the events vector
   */
  [[noreturn]] void listener();

  /**
   * Adds event to the events vector
   */
  void add_event(const Event &event);

  uint32_t round_time; // round time in miliseconds

  /**
   * Sends all the new events to all the players_sorted.
   * Only reads the events vector
   */
  void send_events_queue();

  /**
   * Send the datagram to everyone.
   * Resets the msg vector (clears it, and inserts the game id to it)
   * @param msg
   */
  void send_datagram(std::vector<unsigned char> &msg,
                     const sockaddr_storage *dest);

  /**
   * Send events, starting from next_expected ... up until the most recent one
   * @param next_expected_event
   * @param destination
   */
  void send_events_to(uint32_t next_expected_event,
                      const sockaddr_storage *destination);

public:
  ServerManager(GameState &gameState, int sockfd, float rounds_per_sec)
      : sockfd(sockfd), state(gameState), next_event_to_send(0) {
    round_time = ((float)1000 / rounds_per_sec);

    printf("Server created, round time in miliseconds: %d\n", round_time);
  }

  /**
   * Start the server loop.
   * Reads players_sorted data.
   * Updates events.
   */
  void start();
};

#endif // SERVERMANAGER_H
