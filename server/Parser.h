//
// Created by marek on 23/08/2021.
//

#ifndef BIG_ZADANIE_PARSER_H
#define BIG_ZADANIE_PARSER_H

#include <cstdint>
#include <string>

class Parser {
public:
  struct datagram{
    uint64_t session_id;
    unsigned char turn_direction;
    uint32_t next_expected_event_no;
    std::string name;
  };

  static datagram parse(char *buffer, long len);
};

#endif // BIG_ZADANIE_PARSER_H
