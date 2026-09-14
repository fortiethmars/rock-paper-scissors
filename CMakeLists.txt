cmake_minimum_required(VERSION 3.16)

project(rock_paper_scissors VERSION 2.0.0 LANGUAGES CXX)

option(RPS_BUILD_TESTS "Build the test executable" ON)
option(RPS_TESTS_DURING_BUILD "Run tests as part of the default build" ON)

add_library(rps_core src/game.cpp)
target_include_directories(rps_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(rps_core PUBLIC cxx_std_17)

add_executable(rps src/main.cpp)
target_link_libraries(rps PRIVATE rps_core)

if(MSVC)
    target_compile_options(rps_core PRIVATE /W4 /permissive-)
    target_compile_options(rps PRIVATE /W4 /permissive-)
else()
    target_compile_options(rps_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(rps PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING AND RPS_BUILD_TESTS)
    add_executable(rps_tests tests/test_game.cpp)
    target_link_libraries(rps_tests PRIVATE rps_core)
    target_compile_features(rps_tests PRIVATE cxx_std_17)
    add_test(NAME rps_unit_tests COMMAND rps_tests)

    if(RPS_TESTS_DURING_BUILD)
        add_custom_target(check ALL
            COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>
            DEPENDS rps_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running Rock-Paper-Scissors tests"
            VERBATIM
        )
    endif()
endif()
