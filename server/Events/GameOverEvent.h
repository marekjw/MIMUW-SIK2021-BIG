#ifndef GAMEOVEREVENT_H
#define GAMEOVEREVENT_H

#include "../../util/constants.h"
#include "Event.h"

class GameOverEvent : public Event {
public:
  GameOverEvent() {
    util::serialize(data, htonl((uint32_t)GAME_OVER_LEN));
    data.push_back(GAME_OVER_EVENT);
    calculate_crc();
  }
};

#endif // GAMEOVEREVENT_H
