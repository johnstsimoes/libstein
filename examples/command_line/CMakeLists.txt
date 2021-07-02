cmake_minimum_required(VERSION 3.17)
project(TestStein)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    LibStein 
    GIT_REPOSITORY "git@github.com:johnstsimoes/libstein.git"
)
FetchContent_MakeAvailable(LibStein)
include_directories(${LibStein_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    main.cpp)

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC 
    LibStein)
