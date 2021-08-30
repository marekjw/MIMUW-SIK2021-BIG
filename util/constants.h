#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SEND_TO_SERVER_T 30

#define BUFFER_SIZE 4096
#define GAME_NO_BYTES 4
#define LEN_LEN 4
#define MINIMUM_EVENT__LEN 5
#define CRC32LEN 4

#define LEFT_DIRECTION 2
#define STRAIGHT 0
#define RIGHT_DIRECTION 1

enum msg_from_gui {
  LEFT_KEY_UP,
  LEFT_KEY_DOWN,
  RIGHT_KEY_UP,
  RIGHT_KEY_DOWN,
  UNKNOWN,
};

enum game_number_validity{
  VALID,
  INVALID,
  VALID_NEW_GAME,
};

// event types
#define NEW_GAME_EVENT 0
#define PIXEL_EVENT 1
#define PLAYER_ELIMINATED_EVENT 2
#define GAME_OVER_EVENT 3

// lenghts of messages from server (minus event_no and event_type)
#define PIXEL_EVENT_LEN 9
#define PLAYER_ELIMINATED_EVENT_LEN 1

// lenght of game_over_event (including event_no and event_type)
#define GAME_OVER_LEN 5

#define MAX_CLIENT_DATAGRAM_LEN 33
#define MAX_DATA_FROM_SERVER_LEN 550

// number of players who have to be ready in order for the game to begin
#define MIN_AMOUNT_OF_PLAYERS 2
#define MAX_PLAYERS 25

#define MILISECOND_IN_2_SECONDS 2000

#define MIN_ALIVE_PLAYERS 1

#endif // CONSTANTS_H
