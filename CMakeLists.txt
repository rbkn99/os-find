cmake_minimum_required(VERSION 3.10)
project(os-find)

set(CMAKE_CXX_STANDARD 14)

add_executable(os-find main.cpp params_holder.h params_holder.cpp file_walker.h file_walker.cpp)