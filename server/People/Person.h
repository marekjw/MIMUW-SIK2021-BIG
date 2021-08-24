
#ifndef PERSON_H
#define PERSON_H

#include <netinet/in.h>

class Person {
private:
  uint64_t timestamp;

protected:
  sockaddr_in address;

public:
  Person(sockaddr_in addr) : address(addr) { stamp(); }

  /**
   * Updates the object timestamp with current time
   */
  void stamp();

  uint64_t get_last_activity_timestamp();

  const sockaddr_in &get_address() { return address; }
};

#endif // PERSON_H
