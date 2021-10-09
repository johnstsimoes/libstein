add_executable(libstein_unit
               main.cpp
               arguments_tests.cpp
               command_line_tests.cpp
               command_line_arguments_tests.cpp
               date_utils_tests.cpp
               month_ticker_tests.cpp
               string_utils_tests.cpp)

find_package(GTest CONFIG REQUIRED)

target_link_libraries(libstein_unit
                      libstein
                      fmt
                      gtest)

add_test(NAME libstein_unit COMMAND libstein_unit)
