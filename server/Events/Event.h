#ifndef EVENT_H
#define EVENT_H

#include "../../util/CRC.h"
#include "../../util/util.h"
#include <vector>

class Event {
protected:
  std::vector<unsigned char> data;

  void calculate_crc() {
    util::serialize(
        data, htonl(CRC::calculate(data.data(), data.data() + data.size())));
  }

public:
  std::vector<unsigned char> &get_data() { return this->data; };
};

#endif // EVENT_H
