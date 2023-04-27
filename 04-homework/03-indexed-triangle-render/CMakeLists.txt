cmake_minimum_required(VERSION 3.22)

project(triangles)

add_executable(triangle_renderer
    00-blank-canvas.cpp
    00-blank-canvas.hpp
    01-line-renderer.hpp
    01-line-renderer.cpp
    02-triangle-render.hpp
    02-triangle-render.cpp
    03-indexed-triangle-render-main.cpp
    03-indexed-triangle-render.cpp
    03-indexed-triangle-render.hpp
)