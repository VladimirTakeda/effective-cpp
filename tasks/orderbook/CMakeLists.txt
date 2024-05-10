cmake_minimum_required(VERSION 3.8)

project(orderbook)

set(CMAKE_CXX_STANDARD             20)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(${PROJECT_NAME} main.cpp)

#I had to comment this because if avx-matmul (uncomment please during the check)

#find_package(PkgConfig REQUIRED)
#(lz4 REQUIRED IMPORTED_TARGET liblz4)
#target_link_libraries(${PROJECT_NAME} PkgConfig::lz4)

find_package(gflags REQUIRED)
target_link_libraries(${PROJECT_NAME} ${gflags_LIBRARIES})



#include(FetchContent)
#FetchContent_Declare(
#        Abseil
#        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
#        GIT_TAG master
#)
#FetchContent_MakeAvailable(Abseil)

#target_link_libraries(${PROJECT_NAME} absl::btree absl::hash)

#add_catch(test_pcap_parser test_parser.cpp)
#target_compile_definitions(test_pcap_parser PUBLIC TASK_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
