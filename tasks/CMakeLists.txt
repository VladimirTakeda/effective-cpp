cmake_minimum_required(VERSION 3.15)

project(effective-cpp)

set(CMAKE_CXX_STANDARD             17)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)
set(GOOGLETEST_PATH "${CMAKE_SOURCE_DIR}/contrib/googletest" CACHE PATH "")

find_package(Catch REQUIRED)

add_subdirectory(contrib/benchmark)

include(cmake/TestSolution.cmake)
include(cmake/BuildFlags.cmake)
include_directories(util)

add_subdirectory(multiplication)

add_subdirectory(byte_order)
add_subdirectory(short_view)
add_subdirectory(reverse_vector)
add_subdirectory(static_allocator)
add_subdirectory(set_allocator)

add_subdirectory(local-max)
add_subdirectory(fold)
add_subdirectory(itertools)
add_subdirectory(static-map)
add_subdirectory(intrusive-list)

add_subdirectory(hashmap)
add_subdirectory(deque)
add_subdirectory(cow-vector)
add_subdirectory(lru-cache)

add_subdirectory(dispatch)
add_subdirectory(clever-set)
add_subdirectory(concepts)

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/trades/trades.cmake)
  add_subdirectory(trades)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/png-decoder/build.cmake)
  add_subdirectory(png-decoder)
endif()

add_subdirectory(bgrep)
add_subdirectory(orderbook)

add_subdirectory(reduce)
add_subdirectory(rw-lock)
add_subdirectory(buffered-channel)
#
add_subdirectory(sse-search)
add_subdirectory(avx-matmul)
