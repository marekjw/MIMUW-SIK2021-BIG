//
// Created by marek on 25/08/2021.
//

#ifndef BIG_ZADANIE_NEWGAMEEVENT_H
#define BIG_ZADANIE_NEWGAMEEVENT_H

#include "Event.h"
#include "../People/PlayerState.h"

#include <vector>

class NewGameEvent: public Event{
public:
  NewGameEvent(std::vector<PlayerState> &players);
};

#endif // BIG_ZADANIE_NEWGAMEEVENT_H
