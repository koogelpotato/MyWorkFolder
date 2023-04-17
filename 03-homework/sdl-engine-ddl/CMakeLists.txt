cmake_minimum_required(VERSION 3.22)

project(engine)
find_package(SDL3 REQUIRED)

add_library(engine SHARED
    engine.cpp
    engine.hpp
    keybind.cpp
    keybind.hpp)

target_link_libraries(engine PRIVATE SDL3::SDL3-shared)

add_executable(engine-executable main.cpp)

target_link_libraries(engine-executable PRIVATE engine)




