cmake_minimum_required(VERSION 3.22)
project(myproject)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(lib)

add_executable(myproject src/main.cpp)

target_link_libraries(myproject lib/libgreeting-dynamic.so)


