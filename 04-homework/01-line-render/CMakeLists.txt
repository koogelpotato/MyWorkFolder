cmake_minimum_required(VERSION 3.22)

project(bresenhams-line-render)

add_executable(line-render
    00-blank-canvas.cpp
    00-blank-canvas.hpp
    01-line-renderer.hpp
    01-line-renderer.cpp
    01-line-renderer-main.cpp
)