cmake_minimum_required(VERSION 3.18)
project(tanks)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_subdirectory(src)
add_subdirectory(tests)
