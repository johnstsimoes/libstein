#include <gtest/gtest.h>
#include "libstein/string_utils.h"

using namespace libstein::stringutils;

TEST(StringUtilsTest, split)
{
    EXPECT_EQ(6, split("this,is,a,csv,,123", ',').size());

    auto results = split("This is a string", ' ');
    EXPECT_EQ(4, results.size());
    EXPECT_EQ("This", results[0]);
    EXPECT_EQ("is", results[1]);
    EXPECT_EQ("a", results[2]);
    EXPECT_EQ("string", results[3]);

    EXPECT_EQ(0, split("", ' ').size());
    EXPECT_EQ(1, split(" ", ' ').size());
    EXPECT_EQ(0, split("", 'a').size());
    EXPECT_EQ(1, split("string", '1').size());

}

TEST(StringUtilsTest, get_environment)
{
    setenv("SOME_VARIABLE_93305403245", "expected_value", true);

    EXPECT_EQ("expected_value", get_environment("SOME_VARIABLE_93305403245"));
    EXPECT_NE("", get_environment("PATH"));
    EXPECT_EQ("", get_environment("SOMETHING_UNLIKELY_1234234"));
}