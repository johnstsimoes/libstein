cmake_minimum_required(VERSION 3.17)
project(TestStein)

set(CMAKE_CXX_STANDARD 11)

find_package(fmt CONFIG REQUIRED)

add_subdirectory(src)

# enable_testing()
# include_directories(src)
# add_subdirectory(tests)
