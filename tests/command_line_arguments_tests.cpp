#include <gtest/gtest.h>
#include <vector>
#include "libstein/arguments.h"
#include "libstein/command_line.h"

using namespace libstein;

TEST(CommandLineArgumentsTest, basic_tests)
{
    // this is the equivalent of argc, argv on main
    std::vector<std::string> args = { "--user", "John", "-p", "password", "-h"};

    CommandLine commandLine;
    auto results = commandLine
        .parameter("u,user", "", true)
        .parameter("p", "", true)
        .parameter("h", "", true);
    Arguments arguments(args);
    commandLine.eval(arguments);

    EXPECT_EQ (3, commandLine.count());
    EXPECT_EQ ("John", commandLine.getValue("user")); 
    EXPECT_EQ ("password", commandLine.getValue("p"));

    EXPECT_TRUE (commandLine.isSet("h"));
}

TEST(CommandLineArgumentsTest, params_tests)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password", "-h", "url"};
    
    CommandLine commandLine;
    auto results = commandLine
        .parameter("u,user", "", true)
        .parameter("p", "", true)
        .parameter("h", "", true);
    Arguments arguments(args);
    commandLine.eval(arguments);

    EXPECT_EQ (3, commandLine.count());
    EXPECT_EQ ("John", commandLine.getValue("u"));
    EXPECT_EQ ("password", commandLine.getValue("p"));
    EXPECT_EQ ("url", commandLine.getValue("h"));
    EXPECT_EQ ("", commandLine.getValue("unknown"));
    EXPECT_EQ (true, commandLine.isSet("u"));
    EXPECT_EQ (false, commandLine.isSet("unknown"));
    EXPECT_EQ (false, commandLine.isSet("John"));

    EXPECT_EQ (true, commandLine.hasValue("u"));
    EXPECT_EQ (true, commandLine.hasValue("p"));
    EXPECT_EQ (true, commandLine.hasValue("h"));
    EXPECT_EQ (false, commandLine.hasValue("unknown"));
}

TEST(CommandLineArgumentsTest, edge_cases)
{
    std::vector<std::string> args = { "-u", "-ANOTHER", "-p", "--lval", "rval", "password", "-h", "url"};
    
    CommandLine commandLine;
    auto results = commandLine
        .parameter("u", "", true)
        .parameter("ANOTHER", "", true)
        .parameter("p", "", true)
        .parameter("lval", "", true)
        .parameter("h", "", true);
    Arguments arguments(args);
    commandLine.eval(arguments);

    EXPECT_EQ (5, commandLine.count());
    EXPECT_EQ ("", commandLine.getValue("u"));
    EXPECT_EQ ("rval", commandLine.getValue("lval"));

    EXPECT_EQ (false, commandLine.isSet("U"));

    EXPECT_EQ (true, commandLine.isSet("u"));
    EXPECT_EQ (true, commandLine.isSet("ANOTHER"));
    EXPECT_EQ (true, commandLine.isSet("p"));
    EXPECT_EQ (true, commandLine.isSet("lval"));
    EXPECT_EQ (true, commandLine.isSet("h"));

    EXPECT_EQ (false, commandLine.isSet("password"));
    EXPECT_EQ (false, commandLine.isSet("rval"));
    EXPECT_EQ (false, commandLine.isSet("url"));

    EXPECT_EQ (false, commandLine.hasValue("u"));
    EXPECT_EQ (false, commandLine.hasValue("ANOTHER"));
    EXPECT_EQ (false, commandLine.hasValue("p"));
    EXPECT_EQ (true, commandLine.hasValue("lval"));
    EXPECT_EQ (true, commandLine.hasValue("h"));
}
