project(game_of_life)

cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/bandit/)

add_definitions(-std=c++11 -Wall -Wextra)

execute_process(COMMAND git submodule update --init --recursive)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/bandit/cmake/cotire/CMake/")
include(cotire)

add_executable(gof_tests gof_tests.cpp)

cotire(gof_tests)

enable_testing()

add_test(GameOfLifeTests gof_tests)
