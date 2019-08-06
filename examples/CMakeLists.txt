add_subdirectory("${PROJECT_SOURCE_DIR}/extern/cxxopts" "extern/cxxopts")

add_executable(hello hello.cpp)
target_link_libraries(hello PUBLIC makemeglow)

add_executable(glow_app glow_app.cpp)
target_link_libraries(glow_app PUBLIC cxxopts makemeglow)
