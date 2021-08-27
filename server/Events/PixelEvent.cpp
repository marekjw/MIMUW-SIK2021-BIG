#include "PixelEvent.h"
#include "../../util/constants.h"
#include "../../util/util.h"

PixelEvent::PixelEvent(uint32_t event_no, std::pair<int, int> position,
                       int number) {
  util::serialize(data, htonl((uint32_t)PIXEL_EVENT_LEN));
  util::serialize(data, htonl(event_no));
  data.push_back(PIXEL_EVENT);
  data.push_back(number);                        // only one byte
  util::serialize(data, htonl(position.second)); // y
  util::serialize(data, htonl(position.first));  // x
  calculate_crc();
}
