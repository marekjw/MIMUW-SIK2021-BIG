#ifndef GAMEOVEREVENT_H
#define GAMEOVEREVENT_H

#include "../../util/constants.h"
#include "Event.h"

class GameOverEvent : public Event {
private:
  static const uint32_t len = 5;

public:
  explicit GameOverEvent(uint32_t event_no) {
    util::serialize(data, htonl(len));
    util::serialize(data, htonl(event_no)); // 4 bytes
    data.push_back(GAME_OVER_EVENT);        // 1 byte
    calculate_crc();
  }
};

#endif // GAMEOVEREVENT_H
