cmake_minimum_required(VERSION 3.22)

project(hot-reload)

add_executable(eng engine.cpp engine.hpp main.cpp keybind.hpp keybind.cpp)
find_package(SDL3 REQUIRED)

target_link_libraries(eng PRIVATE SDL3::SDL3-shared)

set_target_properties(
    eng
    PROPERTIES ENABLE_EXPORTS 1 
)

find_package(SDL3 REQUIRED)
target_link_libraries(eng PRIVATE SDL3::SDL3-shared)

add_library(mygame SHARED game.cpp)

target_link_libraries(mygame PRIVATE eng)





