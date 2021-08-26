#include "Person.h"
#include "../../util/constants.h"
#include <chrono>

void Person::stamp() {
  timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();
}
bool Person::should_disconnect() const {
  uint64_t current = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();

  if(current - timestamp > MILISECOND_IN_2_SECONDS){
    return true;
  }
  return false;
}
