cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD             20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")

project(Cache)

add_executable(${PROJECT_NAME} main.cpp)