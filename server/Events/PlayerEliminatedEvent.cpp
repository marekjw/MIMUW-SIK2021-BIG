#include "PlayerEliminatedEvent.h"

#include "../../util/constants.h"
PlayerEliminatedEvent::PlayerEliminatedEvent(int number) {
  data.push_back(PLAYER_ELIMINATED_EVENT_LEN);
  data.push_back(number);
  calculate_crc();
}
