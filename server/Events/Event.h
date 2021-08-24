#ifndef EVENT_H
#define EVENT_H

#include <vector>

class Event {
private:
  std::vector<unsigned char> data;

public:
  std::vector<unsigned char> &get_data() { return this->data; };
};

#endif // EVENT_H
