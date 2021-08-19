#include "StreamBuffer.h"
#include "../util/err.h"
#include <algorithm>
#include <cstring>
#include <sys/socket.h>

StreamBuffer::StreamBuffer(int socket) : socket(socket), index(0), msg_end(0) {
  memset(buffer, 0, BUFFER_SIZE);
}

unsigned char StreamBuffer::getchar() {
  if (index >= msg_end) {
    msg_end = recv(socket, buffer, BUFFER_SIZE, 0);
    if (msg_end < 0)
      syserr("Could not recv from stream socket");
    index = 0;
  }

  return buffer[index++];
}

bool StreamBuffer::match_pattern(const std::string &pattern) {
  return std::all_of(pattern.begin(), pattern.end(),
                     [this](char x) { return x == this->getchar(); });
}
