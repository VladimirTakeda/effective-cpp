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
    set(CMAKE_INSTALL_CONFIG_NAME "ASAN")
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
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/contrib/benchmark/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/multiplication/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/byte_order/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/short_view/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/reverse_vector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/static_allocator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/set_allocator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/local-max/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/fold/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/itertools/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/static-map/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/intrusive-list/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/hashmap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/deque/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/cow-vector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/lru-cache/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/dispatch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/clever-set/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/concepts/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/trades/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/png-decoder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/bgrep/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/orderbook/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/reduce/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/rw-lock/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/buffered-channel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/sse-search/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/avx-matmul/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/vovatakeda/Desktop/CPlusPlusProjects/effective-cpp/tasks/cmake-build-asan/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
