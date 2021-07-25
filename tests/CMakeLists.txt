include(FetchContent)
FetchContent_Declare(
  googletest 
  GIT_REPOSITORY https://github.com/google/googletest.git
#  GIT_TAG        release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# find_package(fmt CONFIG REQUIRED)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(LibStein_tst 
                main.cpp 
                arguments_tests.cpp
                command_line_tests.cpp
                command_line_arguments_tests.cpp
                date_utils_tests.cpp
                month_ticker_tests.cpp
                string_utils_tests.cpp)

target_link_libraries(LibStein_tst 
                      LibStein
                      gtest_main)

add_test(NAME LibStein_test COMMAND LibStein_tst)

