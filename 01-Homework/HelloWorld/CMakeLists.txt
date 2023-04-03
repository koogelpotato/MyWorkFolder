cmake_minimum_required(VERSION 3.0)

project(hello_world)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello_world src/main.cpp)

enable_testing()

add_test(NAME hello_world_test
         COMMAND hello_world)

include(CTest)


