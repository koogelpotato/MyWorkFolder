cmake_minimum_required(VERSION 3.22)

project(engine)
find_package(SDL3 REQUIRED)

add_executable(engine-static main.cpp)

add_library(engine
    engine.cpp
    engine.hpp
    keybind.cpp
    keybind.hpp)

target_link_libraries(engine-static PRIVATE engine)
target_link_libraries(engine-static PRIVATE SDL3::SDL3)


