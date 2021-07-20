cmake_minimum_required(VERSION 3.17)

IF( NOT CMAKE_BUILD_TYPE )
   message ("Release build")
   SET( CMAKE_BUILD_TYPE Release ... FORCE )
ENDIF()

project(LibStein
        VERSION 1.0.0
        DESCRIPTION "John Simoes' C/C++ library for general projects.")

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_subdirectory(src)

enable_testing()
include_directories(src)
add_subdirectory(tests)
