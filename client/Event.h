#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <utility>

class Event {
  uint32_t event_type;
  std::string *data;
public:
  Event(uint32_t type, std::string *data): event_type(type), data(data){}
};

#endif // EVENT_H
