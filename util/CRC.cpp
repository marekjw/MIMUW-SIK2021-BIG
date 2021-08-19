#include "CRC.h"
#include "constants.h"

uint32_t CRC::calculate(unsigned char *it, const unsigned char *end) {
  uint32_t crc32 = UINT32_MAX;
  uint8_t octet;
  for (; it != end; ++it) {
    octet = *it;
    crc32 = (crc32 >> 8) ^ crc32_tab[(crc32 & UINT32_MAX) ^ octet];
  }

  return crc32;
}
