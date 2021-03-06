#include <gtest/gtest.h>
#include "libstein/command_line.h"

using namespace libstein;

TEST(CommandLineTest, basic_test_ok)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password"};

    Arguments arguments(args);
    CommandLine command_line;

    auto results = command_line
        .description("Just an ordinary test.")
        .parameter("u", "", true)
        .parameter("p", "", true)
        .parameter("x,path,PATH", "", true)
        .eval(arguments);

    EXPECT_EQ(0, results.output.size());
    EXPECT_EQ(true, results.valid);
    
    EXPECT_EQ("John", command_line.getValue("u"));
    EXPECT_EQ("password", command_line.getValue("p"));
    EXPECT_NE("", command_line.getValue("x"));

    EXPECT_NO_THROW({ command_line.getValue("unknown"); });
}

TEST(CommandLineTest, basic_test_err)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password", "-h=url"};

    Arguments arguments(args);

    auto results = CommandLine()
        .description("Just an ordinary test.")
        .parameter("a,longa,ENV_LONGA","A value for 'a'", true)
        .parameter("b", "Just another parameter", true)
        .eval(arguments);

    EXPECT_GT(results.output.size(), 0);
    EXPECT_EQ(false, results.valid);
}

TEST(CommandLineTest, test_valid_parameters)
{
    EXPECT_THROW({

        auto results = CommandLine()
            .parameter("0,1,2,3", "Invalid parameters", true);

    }, std::runtime_error);

    EXPECT_THROW({

        auto results = CommandLine()
            .parameter("", "Invalid parameters", true);

    }, std::runtime_error);

    EXPECT_THROW({

        auto results = CommandLine()
            .parameter(",,", "Invalid parameters", true);

    }, std::runtime_error);
}

TEST(CommandLineTest, test_duplicated_parameters)
{
    EXPECT_THROW({

        auto results = CommandLine() 
            .parameter("u,user,ENV_USER", "", true)
            .parameter("u", "", true);

    }, std::runtime_error);

    EXPECT_THROW({

        auto results = CommandLine()
            .parameter("u,user,ENV_USER", "", true)
            .parameter("user", "", true);

    }, std::runtime_error);

    EXPECT_THROW({

        auto results = CommandLine() 
            .parameter("u,user,ENV_USER", "", true)
            .parameter("x,xuser,ENV_USER", "", true);

    }, std::runtime_error);
}

TEST(CommandLineTest, test_environment_vars)
{
    std::vector<std::string> args = { ""};

    Arguments arguments(args);

    auto command_line= CommandLine();

    // Should find PATH
    auto results = command_line 
        .description("Just an ordinary test.")
        .parameter("u,user,PATH", "some description", true)
        .eval(arguments);

    EXPECT_NE("", command_line.getValue("u"));

    EXPECT_EQ(0, results.output.size());
    EXPECT_EQ(true, results.valid);

    // Won't find TESTS20210705
    results = CommandLine()
        .description("Just an ordinary test.")
        .parameter("u,user,TESTS20210705", "some description", true)
        .eval(arguments);

    EXPECT_EQ(false, results.valid);
}

TEST(CommandLineTest, test_long_params)
{
    std::vector<std::string> args = { "--user", "John"};

    Arguments arguments(args);

    auto results = CommandLine()
        .description("Just an ordinary test.")
        .parameter("u,user,ENV_USER", "", true)
        .eval(arguments);

    EXPECT_EQ(0, results.output.size());
    EXPECT_EQ(true, results.valid);
}

TEST(CommandLineTest, edge_cases)
{
    std::vector<std::string> args = { "-u", "John"};

    Arguments arguments(args);
    CommandLine command_line;

    auto results = command_line
        .description("Just an ordinary test.")
        .parameter("u", "", true)
        .parameter("x,path,PATH", "", false) // non-mandatory
        .eval(arguments);

    EXPECT_EQ(0, results.output.size());
    EXPECT_EQ(true, results.valid);
    
    EXPECT_NE("", command_line.getValue("x"));
}
