cmake_minimum_required(VERSION 3.22)

project(sdl-sub)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LOCAL_INCLUDE_DIR $ENV{USR}/local/include)
set(SDL3_LIB_DIR $ENV{USR}/local/lib/libSDL3.so)
find_library(SDL3_LIB SDL3 HINTS ${SDL3_LIB_DIR})

set(TARGET sdl-sub)
set(SRC_DIR src)
add_executable(${TARGET} ${SRC_DIR}/main.cpp)

target_include_directories(${TARGET} PRIVATE ${LOCAL_INCLUDE_DIR})
target_link_libraries(${TARGET} PRIVATE ${SDL3_LIB_DIR})