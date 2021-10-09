add_executable(libstein_integration_tests
               cached_rest_tests.cpp
               main.cpp)

target_link_libraries(libstein_integration_tests
                      libstein
                      hiredis
                      curl
                      fmt
                      gtest)

add_test(NAME libstein_integration_tests COMMAND libstein_integration_tests)
