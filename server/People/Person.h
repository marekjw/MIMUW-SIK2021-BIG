
#ifndef PERSON_H
#define PERSON_H

#include <netinet/in.h>

class Person {
private:
  uint64_t timestamp;
  uint64_t session_id;

protected:
  sockaddr_storage address;

public:
  Person(sockaddr_storage addr, uint64_t session_id)
      : address(addr), session_id(session_id) {
    stamp();
  }

  /**
   * Updates the object timestamp with current time
   */
  void stamp();

  bool should_disconnect() const;

  [[nodiscard]] uint64_t get_session_id() const;

  void update_session_id(uint64_t id) { session_id = id; }

  const sockaddr_storage &get_address() const { return address; }
};

#endif // PERSON_H
