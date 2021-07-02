#include <gtest/gtest.h>
#include "libstein/command_line.h"

using namespace libstein;

TEST(CommandLineTest, basic_test_ok)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password", "h=url"};

    Arguments arguments(args);

    CommandLine commandLine;

    auto results = commandLine
        .description("Just an ordinary test.")
        .parameter("u", "", true)
        .parameter("p", "", true)
        .parameter("h", "", true)
        .eval(arguments);

    EXPECT_EQ(0, results.output.size());
    EXPECT_EQ(true, results.valid);
}

TEST(CommandLineTest, basic_test_err)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password", "h=url"};

    Arguments arguments(args);

    CommandLine commandLine;

    auto results = commandLine
        .description("Just an ordinary test.")
        .parameter("a,longa,ENV_LONGA","A value for 'a'", true)
        .parameter("b", "Just another parameter", false)
        .eval(arguments);

    EXPECT_LT(0, results.output.size());
    EXPECT_EQ(false, results.valid);
}

TEST(CommandLineTest, test_valid_parameters)
{
    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter("0,1,2,3", "Invalid parameters", true);

    }, std::runtime_error);

    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter("", "Invalid parameters", true);

    }, std::runtime_error);

    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter(",,", "Invalid parameters", true);

    }, std::runtime_error);
}

TEST(CommandLineTest, test_duplicated_parameters)
{
    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter("u,user,ENV_USER", "", true)
            .parameter("u", "", true);

    }, std::runtime_error);

    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter("u,user,ENV_USER", "", true)
            .parameter("user", "", true);

    }, std::runtime_error);

    EXPECT_THROW({

        CommandLine commandLine;
        auto results = commandLine
            .parameter("u,user,ENV_USER", "", true)
            .parameter("ENV_USER", "", true);

    }, std::runtime_error);
}
