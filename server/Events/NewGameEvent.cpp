#include "NewGameEvent.h"

#include "../../util/constants.h"
#include "../../util/util.h"

NewGameEvent::NewGameEvent(std::vector<PlayerState> &players, uint32_t max_x,
                           uint32_t max_y) {
  std::vector<unsigned char> temp;

  util::serialize(temp, htonl(max_y));
  util::serialize(temp, htonl(max_x));

  for (auto const &player : players) {
    temp.insert(temp.end(), player.get_name().begin(), player.get_name().end());
    temp.push_back(' ');
  }
  temp.pop_back();

  data.push_back(temp.size());
  data.push_back(NEW_GAME_EVENT);

  data.insert(data.end(), temp.begin(), temp.end());
}
