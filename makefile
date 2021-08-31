CLIENTSRC = client/*.cpp util/CRC.cpp util/err.cpp util/util.cpp
CLIENTHEADERS = client/*.h util/*.h
SERVERSRC = server/*.cpp server/Events/*.cpp server/People/*.cpp util/*.cpp
SERVERHDR = server/*.h server/Events/*.h server/People/*.h util/*.h

CXX = g++

CFLAGS = -Wall -Wextra -std=c++17 -lpthread -O2 -Wno-reorder

.PHONY: all clean

all: screen-worms-client screen-worms-server

screen-worms-client: $(CLIENTSRC) $(CLIENTHEADERS)
	$(CXX) $(CLIENTSRC) $(CFLAGS) -o screen-worms-client

screen-worms-server: $(SERVERSRC) $(SERVERHDR)
	$(CXX) $(SERVERSRC) $(CFLAGS) -o screen-worms-server

clean:
	- rm *.o screen-worms-client screen-worms-server