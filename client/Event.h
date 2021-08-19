#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <utility>

class Event {
  uint32_t event_type;
  std::string *data;
public:
  Event(uint32_t type, std::string *data): event_type(type), data(data){}

  std::string *&get_data_ptr(){
    return data;
  }

  [[nodiscard]] uint32_t get_type() const{
    return event_type;
  }
};

#endif // EVENT_H
