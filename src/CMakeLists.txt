add_library(makemeglow STATIC
    glow.cpp
    png.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(makemeglow ${PNG_LIBRARY})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
target_link_libraries(makemeglow ${FREETYPE_LIBRARIES})

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/stb" "extern/stb")
target_link_libraries(makemeglow stb)
