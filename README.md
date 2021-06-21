# LibStein

Example CMakeLists.txt:

```

include(FetchContent)
FetchContent_Declare(
    LibStein 
    GIT_REPOSITORY "git@github.com:johnstsimoes/libstein.git"
)
FetchContent_MakeAvailable(LibStein)
include_directories(${LibStein_SOURCE_DIR}/include)

add_library(${PROJECT_NAME}_lib "")
target_sources(${PROJECT_NAME}_lib PUBLIC
    subdir/some_util.cpp
    basic.cpp)
add_executable(${PROJECT_NAME}
    main.cpp)

target_link_libraries(${PROJECT_NAME}
    LINK_PUBLIC 
    fmt::fmt
    LibStein 
    ${PROJECT_NAME}_lib)
```