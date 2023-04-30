cmake_minimum_required(VERSION 3.22)

project(null-div)

add_executable(test test.cpp)

target_compile_features(test PUBLIC cxx_std_98)