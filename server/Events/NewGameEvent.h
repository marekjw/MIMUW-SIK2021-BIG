#ifndef NEWGAMEEVENT_H
#define NEWGAMEEVENT_H

#include "Event.h"
#include "../People/PlayerState.h"

#include <vector>

class NewGameEvent: public Event{
public:
  NewGameEvent(uint32_t event_no, std::vector<PlayerState *> &players,
               uint32_t max_y, uint32_t max_x);
};

#endif // NEWGAMEEVENT_H
