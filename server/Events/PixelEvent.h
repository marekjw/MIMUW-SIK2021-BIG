#ifndef PIXELEVENT_H
#define PIXELEVENT_H

#include "Event.h"

#include <utility>

class PixelEvent: public Event{
public:
  PixelEvent(uint32_t event_no, std::pair<int, int> position, int number);
};

#endif // PIXELEVENT_H
