#ifndef PLAYERELIMINATED_H
#define PLAYERELIMINATED_H

#include "Event.h"

class PlayerEliminatedEvent : public Event {
private:
  static const uint32_t len = 5;
public:
  PlayerEliminatedEvent(uint32_t event_no, int number);
};

#endif // PLAYERELIMINATED_H
