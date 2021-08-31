#include "Random.h"
uint32_t Random::operator()() {
  uint32_t res = r;
  r = (r * 279410273) % 4294967291;
  return res;
}
