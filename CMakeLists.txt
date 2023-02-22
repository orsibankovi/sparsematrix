cmake_minimum_required(VERSION 3.23)
project(NHF1_skeleton)

set(CMAKE_CXX_STANDARD 20)

add_executable(NHF1_skeleton main.cpp sparsematrix.hpp sparsematrix.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic")