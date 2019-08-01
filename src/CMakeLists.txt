add_library(makemeglow STATIC
    glow.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory("${PROJECT_SOURCE_DIR}/extern/stb" "extern/stb")
target_link_libraries(makemeglow stb)
