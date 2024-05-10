# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-src"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-build"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/tmp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/src/abseil-populate-stamp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/src"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/src/abseil-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/src/abseil-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-release/_deps/abseil-subbuild/abseil-populate-prefix/src/abseil-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
