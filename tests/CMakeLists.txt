add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME}
        makemeglow
        gtest gmock gtest_main
        ${PNG_LIBRARY} ${FREETYPE_LIBRARIES})
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# pass test data path
set(TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
configure_file(test_data_path.h.in test_data_path.h @ONLY)

include_directories(${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})

package_add_test(glowtest
    color_test.cpp
    font_rasterizer_test.cpp
    gauss_test.cpp
    png_test.cpp
    run_tests.cpp)
