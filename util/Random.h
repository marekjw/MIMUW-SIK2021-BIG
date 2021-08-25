#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
class Random {
private:
  uint64_t r;

public:
  explicit Random(int seed): r(seed){}

  int operator()();
};

#endif // RANDOM_H
