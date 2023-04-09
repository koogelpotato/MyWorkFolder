cmake_minimum_required(VERSION 3.22)

project(MyProject)

find_package(SDL3 REQUIRED)

if(TARGET SDL3::SDL3-static)
    get_target_property(PATH_TO_SDL3_STATIC SDL3::SDL3-static LOCATION)
    message(STATUS "PATH TO libsdl3.a static is: [${PATH_TO_SDL3_STATIC}] ")
else()
    message(FATALL_ERROR "Error: find_library() did not find libsdl3.a")
endif()

add_executable(myapp src/main.cpp)

target_include_directories(myapp PRIVATE include)

target_link_libraries(myapp PRIVATE SDL3::SDL3-static)