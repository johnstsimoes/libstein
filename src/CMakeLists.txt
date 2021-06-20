find_package(fmt CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    arguments.cpp)

# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
# install(FILES arguments.h DESTINATION include)
