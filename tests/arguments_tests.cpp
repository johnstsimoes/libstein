#include <gtest/gtest.h>
#include <vector>
#include "libstein/arguments.h"

using libstein::Arguments;

TEST(ArgumentsTest, basic_tests)
{
    // this is the equivalent of argc, argv on main
    std::vector<std::string> args = { "--user", "John", "-p", "password", "-h=url"};

    Arguments arguments(args);

    EXPECT_EQ (3, arguments.size());
    EXPECT_EQ ("John", arguments.getParameter("user"));   // most basic use
    EXPECT_EQ ("password", arguments.getParameter("-p")); // - also works
    EXPECT_EQ ("url", arguments.getParameter("h"));       // = also works
}

TEST(ArgumentsTest, params_tests)
{
    std::vector<std::string> args = { "-u", "John", "-p", "password", "-h", "url"};
    
    Arguments arguments(args);
    EXPECT_EQ (3, arguments.size());
    EXPECT_EQ ("John", arguments.getParameter("u"));
    EXPECT_EQ ("John", arguments.getParameter("-u"));
    EXPECT_EQ ("password", arguments.getParameter("p"));
    EXPECT_EQ ("url", arguments.getParameter("h"));
    EXPECT_EQ ("", arguments.getParameter("unknown"));
    EXPECT_EQ (true, arguments.isSet("u"));
    EXPECT_EQ (false, arguments.isSet("unknown"));
    EXPECT_EQ (false, arguments.isSet("John"));

    EXPECT_EQ (true, arguments.hasValue("u"));
    EXPECT_EQ (true, arguments.hasValue("p"));
    EXPECT_EQ (true, arguments.hasValue("h"));
    EXPECT_EQ (false, arguments.hasValue("unknown"));

}

TEST(ArgumentsTest, classic_args)
{
    const char* var[] = { "-u", "John", "-p", "password", "-h", "url"};

    Arguments arguments(6, (char**)var);
    EXPECT_EQ (3, arguments.size());
    EXPECT_EQ ("John", arguments.getParameter("u"));
    EXPECT_EQ ("John", arguments.getParameter("-u"));
    EXPECT_EQ ("password", arguments.getParameter("p"));
    EXPECT_EQ ("url", arguments.getParameter("h"));
    EXPECT_EQ ("", arguments.getParameter("unknown"));
    EXPECT_EQ (true, arguments.isSet("u"));
    EXPECT_EQ (false, arguments.isSet("unknown"));
    EXPECT_EQ (false, arguments.isSet("John"));

    EXPECT_EQ (true, arguments.hasValue("u"));
    EXPECT_EQ (true, arguments.hasValue("p"));
    EXPECT_EQ (true, arguments.hasValue("h"));
    EXPECT_EQ (false, arguments.hasValue("unknown"));
}

TEST(ArgumentsTest, edge_cases)
{
    std::vector<std::string> args = { "-u", "-ANOTHER", "-p", "--lval=rval", "password", "-h", "url"};
    
    Arguments arguments(args);
    EXPECT_EQ (5, arguments.size());
    EXPECT_EQ ("", arguments.getParameter("u"));
    EXPECT_EQ ("rval", arguments.getParameter("lval"));

    EXPECT_EQ (false, arguments.isSet("U"));

    EXPECT_EQ (true, arguments.isSet("u"));
    EXPECT_EQ (true, arguments.isSet("ANOTHER"));
    EXPECT_EQ (true, arguments.isSet("p"));
    EXPECT_EQ (true, arguments.isSet("lval"));
    EXPECT_EQ (true, arguments.isSet("h"));

    EXPECT_EQ (false, arguments.isSet("password"));
    EXPECT_EQ (false, arguments.isSet("rval"));
    EXPECT_EQ (false, arguments.isSet("url"));

    EXPECT_EQ (false, arguments.hasValue("u"));
    EXPECT_EQ (false, arguments.hasValue("ANOTHER"));
    EXPECT_EQ (false, arguments.hasValue("p"));
    EXPECT_EQ (true, arguments.hasValue("lval"));
    EXPECT_EQ (true, arguments.hasValue("h"));
}
