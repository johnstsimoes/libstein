add_library(${PROJECT_NAME} SHARED STATIC
    arguments.cpp)

# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
# install(FILES ../include/arguments.h DESTINATION include)

# add_library(${PROJECT_NAME})

# target_include_directories(${PROJECT_NAME} PUBLIC
#     $<BUILD_INTERFACE:${CMAKE_ROOT}/include>
#     $<INSTALL_INTERFACE:include>
#     PRIVATE src)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VERSION ${PROJECT_VERSION}
#     SOVERSION 1)

# install(TARGETS ${PROJECT_NAME} EXPORT LibSteinConfig
#     ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

# install(DIRECTORY include/ 
#         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# install(EXPORT LibSteinConfig DESTINATION share/LibStein/cmake)

# export(TARGETS ${PROJECT_NAME} FILE LibSteinConfig.cmake)