cmake_minimum_required(VERSION 3.22)

project(hot_reload)

add_executable(eng engine.cxx engine.hxx)
target_compile_features(eng PUBLIC cxx_std_20)

set_target_properties(
    eng
    PROPERTIES ENABLE_EXPORTS
               1 
    )

find_package(SDL3 REQUIRED)
target_link_libraries(eng PRIVATE SDL3::SDL3-shared)

add_library(game SHARED game.cxx)
target_compile_features(game PUBLIC cxx_std_20)
target_link_libraries(game PRIVATE eng)