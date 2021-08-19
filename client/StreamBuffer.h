#ifndef STREAMBUFFER_H
#define STREAMBUFFER_H

#include "../util/constants.h"
#include <cstdio>
#include <cstring>
#include <string>

class StreamBuffer {
private:
  unsigned char buffer[BUFFER_SIZE]{};
  int socket;
  ssize_t index, msg_end;

public:
  explicit StreamBuffer(int socket);

  unsigned char getchar();

  bool match_pattern(const std::string &pattern);
};

#endif // STREAMBUFFER_H
