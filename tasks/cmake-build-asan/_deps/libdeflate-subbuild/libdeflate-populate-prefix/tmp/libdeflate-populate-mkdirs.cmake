# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-src"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-build"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/tmp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/src/libdeflate-populate-stamp"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/src"
  "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/src/libdeflate-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/src/libdeflate-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/_deps/libdeflate-subbuild/libdeflate-populate-prefix/src/libdeflate-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
