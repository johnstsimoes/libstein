add_executable(libstein_integration_tests
                main.cpp)

target_link_libraries(libstein
                      curl
                      fmt)

add_test(NAME libstein_integration_tests COMMAND libstein_integration_tests)
