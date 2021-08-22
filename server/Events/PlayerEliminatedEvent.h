#ifndef PLAYERELIMINATED_H
#define PLAYERELIMINATED_H

#include "Event.h"

class PlayerEliminatedEvent : public Event {
public:
  explicit PlayerEliminatedEvent(int number);
};

#endif // PLAYERELIMINATED_H
