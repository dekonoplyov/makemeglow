cmake_minimum_required(VERSION 3.0)
project(makemeglow
    DESCRIPTION "Glowing text rendering library"
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wcast-align")
add_compile_options(-std=c++14)

add_subdirectory(examples)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
