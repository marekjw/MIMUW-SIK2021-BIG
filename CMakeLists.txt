cmake_minimum_required(VERSION 3.19)
project(big_zadanie)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(server server-main.cpp)
add_executable(client client-main.cpp client-state.h err.h err.cpp ClientManager.cpp ClientManager.h StreamBuffer.cpp StreamBuffer.h constants.h)