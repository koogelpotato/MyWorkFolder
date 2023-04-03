cmake_minimum_required(VERSION 3.22)
project(myproject)

add_executable(myproject src/main.cpp)
target_link_libraries(myproject /usr/local/lib/libgreeting-static.a)
