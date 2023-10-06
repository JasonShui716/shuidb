cmake_minimum_required(VERSION 3.15)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -stdlib=libc++ -std=c++20 -Wall")
project(shuidb)

add_library(linenoise external/linenoise/linenoise.c)
target_include_directories(linenoise PUBLIC external/linenoise)

add_library(libshuidb OBJECT)
file(GLOB SRC_FILES "src/**")
set_property(TARGET libshuidb PROPERTY POSITION_INDEPENDENT_CODE 1)

target_sources(
  libshuidb
  PRIVATE
  ${SRC_FILES}
)

target_include_directories(
  libshuidb
  PUBLIC
  include/
)

add_library(libshuidbShared SHARED)
add_library(libshuidbStatic STATIC)
set_target_properties(libshuidbShared PROPERTIES OUTPUT_NAME shuidb)
set_target_properties(libshuidbStatic PROPERTIES OUTPUT_NAME shuidb)
target_link_libraries(libshuidbShared libshuidb)
target_link_libraries(libshuidbStatic libshuidb)

add_executable(shuidb shuidb.cpp)
target_link_libraries(shuidb linenoise libshuidb)

# CTest related
enable_testing()
add_subdirectory(examples)
add_subdirectory(test)