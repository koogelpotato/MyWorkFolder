cmake_minimum_required(VERSION 3.22)

project(triangles)

add_executable(triangle_renderer
    00-blank-canvas.cpp
    00-blank-canvas.hpp
    01-line-render.hpp
    01-line-render.cpp
    02-triangle-render.hpp
    02-triangle-render.cpp
    03-triangle-indexed-render.cpp
    03-triangle-indexed-render.cpp
    05-shader.cpp
    04-interpolated-triangle-render.cpp
    04-interpolated-triangle-render.hpp
)

find_package(SDL3 REQUIRED)

target_link_libraries(triangle_renderer PRIVATE SDL3::SDL3-shared)