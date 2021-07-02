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
        .parameter("u","", true)
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

