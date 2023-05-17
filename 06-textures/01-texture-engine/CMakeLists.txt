cmake_minimum_required(VERSION 3.22)

project(project_engine)

add_library(image_engine
    game.hpp
    resource-manager.hpp
    shader.hpp
    sprite-renderer.hpp
    stb_image.h
    texture.hpp
    glad/glad.c
    glad/glad.h
    KHR/khrplatform.h
    glm/glm.hpp
    resource-manager.cpp
    shader.cpp
    sprite-renderer.cpp
    texture.cpp
    )  

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(image_engine PRIVATE SDL3::SDL3-shared OpenGL::GL)

add_executable(image game.cpp)

target_link_libraries(image PRIVATE image_engine)