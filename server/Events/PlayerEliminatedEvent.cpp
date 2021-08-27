#include "PlayerEliminatedEvent.h"

#include "../../util/constants.h"
PlayerEliminatedEvent::PlayerEliminatedEvent(int number) {
  util::serialize(data, htonl((uint32_t)PLAYER_ELIMINATED_EVENT_LEN));
  data.push_back(number);
  calculate_crc();
}
