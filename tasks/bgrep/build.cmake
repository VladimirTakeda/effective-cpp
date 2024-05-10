add_executable(bin-grep main.cpp)

target_link_libraries(bin-grep stdc++fs)

find_package(gflags REQUIRED)
target_link_libraries(bin-grep gflags)

