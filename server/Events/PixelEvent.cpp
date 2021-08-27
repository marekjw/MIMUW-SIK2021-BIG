#include "PixelEvent.h"
#include "../../util/constants.h"
#include "../../util/util.h"

PixelEvent::PixelEvent(uint32_t event_no, std::pair<int, int> position,
                       int number) {
  util::serialize(data, htonl(len));
  util::serialize(data, htonl(event_no));        // 4 bytes
  data.push_back(PIXEL_EVENT);                   // 1 byte
  data.push_back(number);                        // only one byte
  util::serialize(data, htonl(position.second)); // y 4 bytes
  util::serialize(data, htonl(position.first));  // x 4 bytes
  calculate_crc();
}
