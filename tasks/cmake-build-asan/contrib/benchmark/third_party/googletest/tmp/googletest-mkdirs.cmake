# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/contrib/googletest"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/build"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/tmp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/src/googletest-stamp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/download"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/src/googletest-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/src/googletest-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/third_party/googletest/src/googletest-stamp${cfgdir}") # cfgdir has leading slash
endif()
