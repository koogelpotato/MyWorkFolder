cmake_minimum_required(VERSION 3.22)
project(greeting VERSION 1.0)

add_library(greeting-static STATIC include/greeting.hpp src/greeting.cpp)
target_include_directories(greeting-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(greeting-dynamic SHARED include/greeting.hpp src/greeting.cpp)
target_include_directories(greeting-dynamic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS greeting-static greeting-dynamic
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include)