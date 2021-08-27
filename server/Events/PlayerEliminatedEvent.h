#ifndef PLAYERELIMINATED_H
#define PLAYERELIMINATED_H

#include "Event.h"

class PlayerEliminatedEvent : public Event {
public:
  PlayerEliminatedEvent(uint32_t event_no, int number);
};

#endif // PLAYERELIMINATED_H
