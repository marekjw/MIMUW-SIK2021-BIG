#include "PlayerEliminatedEvent.h"

#include "../../util/constants.h"
PlayerEliminatedEvent::PlayerEliminatedEvent(uint32_t event_no, int number) {
  util::serialize(data, htonl((uint32_t)PLAYER_ELIMINATED_EVENT_LEN));
  util::serialize(data, htonl(event_no));
  data.push_back(number);
  calculate_crc();
}
