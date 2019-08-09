cmake_minimum_required(VERSION 3.0)
project(makemeglow
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wcast-align")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

add_subdirectory(examples)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
