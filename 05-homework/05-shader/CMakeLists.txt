cmake_minimum_required(VERSION 3.22)

project(blank_canvas)


add_library(glad SHARED
    glcontext.cpp
    glcontext.hpp
    glad/glad.h
    glad/glad.c
    KHR/khrplatform.h)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SHADER shader/*.*)
file(COPY ${SHADER} DESTINATION ./shader/)

target_link_libraries(glad PRIVATE OpenGL::GL SDL3::SDL3-shared)

add_executable(blank_canvas
    main.cpp)

target_link_libraries(blank_canvas PRIVATE glad)