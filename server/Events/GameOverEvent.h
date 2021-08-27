#ifndef GAMEOVEREVENT_H
#define GAMEOVEREVENT_H

#include "../../util/constants.h"
#include "Event.h"

class GameOverEvent : public Event {
public:
  explicit GameOverEvent(uint32_t event_no) {
    util::serialize(data, htonl((uint32_t)GAME_OVER_LEN));
    util::serialize(data, htonl(event_no));
    data.push_back(GAME_OVER_EVENT);
    calculate_crc();
  }
};

#endif // GAMEOVEREVENT_H
