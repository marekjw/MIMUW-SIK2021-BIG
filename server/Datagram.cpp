#include "Datagram.h"
#include "endian.h"

Datagram::Datagram(unsigned char *buffer, long len) {
  session_id = be64toh(*(reinterpret_cast<uint64_t *>(buffer)));

  turn_direction = buffer[8];
  next_expected_event_no = util::read_uint32_from_network_stream(buffer + 9);

  name = "";

  for (long i = 13; i < len; ++i) {
    name.push_back((char)buffer[i]);
  }
}
