cmake_minimum_required(VERSION 3.22)

project(myproject)

find_package(SDL3 REQUIRED)

add_executable(myexecutable src/main.cpp)

target_link_libraries(myexecutable PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libengine.a)
target_link_libraries(myexecutable PRIVATE SDL3::SDL3)

target_include_directories(myexecutable PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    lib 
    include
    src)