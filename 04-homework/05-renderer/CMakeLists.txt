cmake_minimum_required(VERSION 3.22)

project(triangles)

add_executable(triangle_renderer
    00-blank-canvas.cpp
    00-blank-canvas.hpp
    01-line-renderer.hpp
    01-line-renderer.cpp
    02-triangle-render.hpp
    02-triangle-render.cpp
    03-indexed-triangle-render.cpp
    03-indexed-triangle-render.hpp
    04-interpolated-triangle-main.cpp
    04-interpolated-triangle.cpp
    04-interpolated-triangle.hpp
)

find_package(SDL3 REQUIRED)

target_link_libraries(triangle_renderer PRIVATE SDL3::SDL3-shared)