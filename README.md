# LibStein

[![CircleCI](https://circleci.com/gh/johnstsimoes/libstein.svg?style=shield&circle-token=cbaed39aa3833480914921853063e56e85cf172a)](<LINK>)

Yet another C++ library for pet projects. Use `#include <libstein.h>`.

## Command line helpers

```
    libstein::Arguments args(argc, argv);

    libstein::CommandLine cmd;

    auto results = cmd.description("Just an ordinary test.")
        .parameter("a,longa,ENV_LONGA","A value for 'a'", true)
        .parameter("b", "Just another parameter", false)
        .eval(args);

    for (auto& line : results.output)
    {
        std::cout << line << std::endl;
    }

    if (results.valid)
    {
        std::cout << "Success" << std::endl;
        exit(EXIT_SUCCESS);
    }
```

## Date and time helpers

```
    const std::tm parse_date(const std::string& date);

    bool is_weekday(const std::tm &date);
    bool is_same_day(const std::tm &day1, const std::tm &day2);
    bool is_same_month(const std::tm &date1, const std::tm &date2);

    int elapsed_days_without_weekends(const std::tm &begin, const std::tm &end);
    std::tm get_next_day(const std::tm &date);
    std::tm beginning_next_month(const std::tm& date);

    int compare_months(const std::tm &date1, const std::tm &date2);
```

## String Utils

```
    std::vector<std::string> split(const std::string &string, const char delimiter);
    std::string get_environment(std::string environment_variable);
```

# 
Example CMakeLists.txt:

```
include(FetchContent)
FetchContent_Declare(
    LibStein 
    GIT_REPOSITORY "git@github.com:johnstsimoes/libstein.git"
)
FetchContent_MakeAvailable(LibStein)
include_directories(${LibStein_SOURCE_DIR}/include)

# other directives

# Don't forget to add LibStein to target_link_libraries.
```
