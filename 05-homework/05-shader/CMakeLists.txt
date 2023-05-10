cmake_minimum_required(VERSION 3.22)

project(blank_canvas)


add_library(glad SHARED
    glad/glad.h
    glad/glad.c
    KHR/khrplatform.h)

target_include_directories(glad PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)


target_link_libraries(glad PRIVATE OpenGL::GL)

add_executable(blank_canvas
    glcontext.cpp
    main.cpp)

target_link_libraries(blank_canvas PRIVATE SDL3::SDL3-shared)

target_link_libraries(blank_canvas PRIVATE glad)