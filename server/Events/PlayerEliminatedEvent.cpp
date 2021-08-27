#include "PlayerEliminatedEvent.h"

#include "../../util/constants.h"
PlayerEliminatedEvent::PlayerEliminatedEvent(uint32_t event_no, int number) {
  util::serialize(data, htonl(len));
  util::serialize(data, htonl(event_no)); // 4 bytes
  data.push_back(number); // 1 byte
  calculate_crc();
}
