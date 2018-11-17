cmake_minimum_required(VERSION 3.12)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp blockchain.cpp blockchain.h picosha2.h)