#include "Random.h"
int Random::operator()() {
  int res = r;
  r = (r * 279410273) % 4294967291;
  return res;
}
