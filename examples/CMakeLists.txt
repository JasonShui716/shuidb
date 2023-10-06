cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -stdlib=libc++ -std=c++20 -Wall")

add_executable(hello_world hello_world.cpp)
target_link_options(hello_world PRIVATE -fno-pie)
