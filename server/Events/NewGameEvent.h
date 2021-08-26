#ifndef NEWGAMEEVENT_H
#define NEWGAMEEVENT_H

#include "Event.h"
#include "../People/PlayerState.h"

#include <vector>

class NewGameEvent: public Event{
public:
  NewGameEvent(std::vector<PlayerState> &players, uint32_t max_x,
                        uint32_t max_y);
};

#endif // NEWGAMEEVENT_H
