cmake_minimum_required(VERSION 3.17)
project(TestStein)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME}
    main.cpp)

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC
    curl
    hiredis
    libstein)
