#include "NewGameEvent.h"

#include "../../util/constants.h"
#include "../../util/util.h"

NewGameEvent::NewGameEvent(uint32_t event_no, std::vector<PlayerState> &players,
                           uint32_t max_y, uint32_t max_x) {
  std::vector<unsigned char> temp;

  util::serialize(temp, htonl(max_y));
  util::serialize(temp, htonl(max_x));

  for (auto const &player : players) {
    temp.insert(temp.end(), player.get_name().begin(), player.get_name().end());
    temp.push_back(' ');
  }
  temp.pop_back();

  // + 5 ----> 1 byte event type, 4 bytes event no
  util::serialize(data, htonl((uint32_t)temp.size() + 5));
  util::serialize(data, htonl(event_no));
  data.push_back(NEW_GAME_EVENT);

  data.insert(data.end(), temp.begin(), temp.end());
  calculate_crc();
}
