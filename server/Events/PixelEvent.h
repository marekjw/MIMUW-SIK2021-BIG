#ifndef PIXELEVENT_H
#define PIXELEVENT_H

#include "Event.h"

#include <utility>

class PixelEvent: public Event{
public:
  PixelEvent(int number, std::pair<int, int> position);
};

#endif // PIXELEVENT_H
