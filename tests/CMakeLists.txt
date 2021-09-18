add_executable(LibStein_tst
                main.cpp
                arguments_tests.cpp
                cached_rest_tests.cpp
                command_line_tests.cpp
                command_line_arguments_tests.cpp
                date_utils_tests.cpp
                month_ticker_tests.cpp
                string_utils_tests.cpp)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(LibStein_tst
                      LibStein
                      hiredis
                      cpr
                      fmt
                      gtest)

add_test(NAME LibStein_test COMMAND LibStein_tst)
