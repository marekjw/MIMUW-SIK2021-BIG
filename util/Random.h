#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
class Random {
private:
  uint64_t r;

public:
  explicit Random(uint32_t seed): r(seed){}

  uint32_t operator()();
};

#endif // RANDOM_H
