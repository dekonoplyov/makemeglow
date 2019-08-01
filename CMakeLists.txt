cmake_minimum_required(VERSION 3.0)
project(makemeglow
    DESCRIPTION "Glowing text rendering library"
    LANGUAGES CXX)

add_compile_options(-std=c++14)

add_subdirectory(examples)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
