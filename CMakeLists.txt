cmake_minimum_required(VERSION 3.19)
project(big_zadanie)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(server server/server-main.cpp util/CRC.cpp util/CRC.h util/util.cpp util/util.h server/ServerManager.cpp server/ServerManager.h)
add_executable(client client/client-main.cpp client/ClientState.h client/ClientState.cpp util/err.h util/err.cpp client/ClientManager.cpp client/ClientManager.h client/StreamBuffer.cpp client/StreamBuffer.h util/constants.h util/CRC.cpp util/CRC.h util/util.cpp util/util.h client/ClientState.cpp client/Event.cpp client/Event.h)