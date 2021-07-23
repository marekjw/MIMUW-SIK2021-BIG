cmake_minimum_required(VERSION 3.19)
project(big_zadanie)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server-main.cpp)
add_executable(client client-main.cpp client-state.h)