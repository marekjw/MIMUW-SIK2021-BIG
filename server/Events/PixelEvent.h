//
// Created by marek on 22/08/2021.
//

#ifndef BIG_ZADANIE_PIXELEVENT_H
#define BIG_ZADANIE_PIXELEVENT_H

#include "Event.h"

#include <utility>

class PixelEvent: public Event{
public:
  PixelEvent(int number, std::pair<int, int> position);
};

#endif // BIG_ZADANIE_PIXELEVENT_H
