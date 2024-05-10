cmake_minimum_required(VERSION 3.15)

project(tic-tac-toe)

set(CMAKE_CXX_STANDARD             17)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

include(../cmake/BuildFlags.cmake)
include_directories(my_strategy)

set(SOURCES
        my_strategy/my_strategy.cpp
)

add_library(opt  SHARED ${SOURCES})

add_executable(runner utility/main.cpp)