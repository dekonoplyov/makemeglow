add_library(makemeglow STATIC
    color.cpp
    font_rasterizer.cpp
    gauss.cpp
    glow.cpp
    png.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(makemeglow ${PNG_LIBRARY})
target_link_libraries(makemeglow ${FREETYPE_LIBRARIES})
