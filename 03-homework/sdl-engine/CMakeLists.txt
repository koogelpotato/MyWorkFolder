cmake_minimum_required(VERSION 3.22)

project(engine)
find_package(SDL3 REQUIRED)

add_executable(engine-static src/main.cpp)

target_link_libraries(engine-static PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libengine.a)
target_link_libraries(engine-static PRIVATE SDL3::SDL3)
target_include_directories(engine-static PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
