add_library(libstein SHARED
    arguments.cpp
    cached_rest.cpp
    command_line.cpp
    csv_reader.cpp
    date_utils.cpp
    month_ticker.cpp
    string_utils.cpp)

target_link_libraries(libstein PRIVATE curl hiredis)
