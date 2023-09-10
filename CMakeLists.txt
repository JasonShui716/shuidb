cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
project(shuidb)

add_library(linenoise external/linenoise/linenoise.c)
target_include_directories(linenoise PUBLIC external/linenoise)

file(GLOB UTIL_FILES "utils/**")

file(GLOB SRC_FILES "src/*.cpp")
add_executable(shuidb ${UTIL_FILES} ${SRC_FILES})
target_include_directories(shuidb PUBLIC utils)
target_compile_options(shuidb PRIVATE -g)

target_link_libraries(shuidb linenoise)

add_executable(hello_world examples/hello_world.cpp)
target_link_options(hello_world PRIVATE -fno-pie)
