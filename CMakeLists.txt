cmake_minimum_required(VERSION 3.14)
project(chess)

set(CMAKE_CXX_STANDARD 14)

add_executable(chess main.cpp utils.h utils.cpp move.h move.cpp board.h board.cpp)