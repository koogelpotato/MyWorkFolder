cmake_minimum_required(VERSION 3.22)

project(MyProject)

find_package(SDL3 REQUIRED)

add_executable(myapp src/main.cpp)

target_include_directories(myapp PRIVATE include)

target_link_libraries(myapp PRIVATE SDL3::SDL3)
