#include "Random.h"
int Random::operator()() {
  int res = r;
  r = (r * 279410273ull) % 4294967291ull;
  return res;
}
