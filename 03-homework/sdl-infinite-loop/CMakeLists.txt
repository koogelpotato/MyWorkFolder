cmake_minimum_required(VERSION 3.22)

project(infinite-loop)

find_package(SDL3 REQUIRED)

include_directories(
    src
    include
    lib
)

add_executable(infinite-loop src/main.cpp)

target_include_directories(infinite-loop PRIVATE
    include
    src
    lib
)

target_link_libraries(infinite-loop PRIVATE SDL3::SDL3)