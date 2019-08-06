add_library(makemeglow STATIC
    color.cpp
    color_buffer.cpp
    font_rasterizer.cpp
    gauss.cpp
    glow.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(makemeglow ${PNG_LIBRARY})
target_link_libraries(makemeglow ${FREETYPE_LIBRARIES})
