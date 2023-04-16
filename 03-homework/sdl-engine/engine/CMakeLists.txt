cmake_minimum_required(VERSION 3.22)

project(engine)

# Collect all the source files in the current directory and its subdirectories
file(GLOB_RECURSE ENGINE_SRC_FILES
    "*.cpp"
    "*.hpp"
)

# Create the library target
add_library(engine STATIC ${ENGINE_SRC_FILES})

# Include the headers in the project
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

