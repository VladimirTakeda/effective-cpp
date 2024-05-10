# Install script for directory: /Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "TSAN")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/contrib/benchmark/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/multiplication/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/byte_order/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/short_view/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/reverse_vector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/static_allocator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/set_allocator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/local-max/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/fold/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/itertools/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/static-map/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/intrusive-list/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/hashmap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/deque/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/cow-vector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/lru-cache/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/dispatch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/clever-set/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/concepts/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/trades/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/png-decoder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/bgrep/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/orderbook/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/reduce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/rw-lock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/buffered-channel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/sse-search/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/avx-matmul/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-tsan/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
