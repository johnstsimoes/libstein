cmake_minimum_required(VERSION 3.16)

IF( NOT CMAKE_BUILD_TYPE )
   message ("Release build")
   SET( CMAKE_BUILD_TYPE Release ... FORCE )
ENDIF()

project(libstein
        VERSION 1.0.0
        DESCRIPTION "John Simoes' C/C++ library for general projects.")

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
add_subdirectory(src)

set(PUBLIC_HEADERS
    ${PROJECT_SOURCE_DIR}/include/libstein.h
)

set_target_properties(libstein PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(libstein PROPERTIES SOVERSION 1)
set_target_properties(libstein PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

include(GNUInstallDirs)
install(TARGETS libstein
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/libstein"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.h"
)

enable_testing()
add_subdirectory(tests)
