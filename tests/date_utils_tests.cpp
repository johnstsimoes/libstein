#include <gtest/gtest.h>
#include "libstein/date_utils.h"

using namespace libstein::dateutils;

TEST(DateUtilsTest, parse_date)
{
    std::tm date = parse_date("15/1/2005");

    EXPECT_EQ (15, date.tm_mday);   // one-based
    EXPECT_EQ (0,  date.tm_mon);    // zero-based
    EXPECT_EQ (105, date.tm_year);  // 1900 + 105 = 2005

    EXPECT_NO_THROW({ parse_date("21/1/1999"); });
    EXPECT_THROW({ parse_date("33/15/1999"); }, std::runtime_error);
}

TEST(DateUtilsTest, is_weekday)
{
    EXPECT_FALSE (is_weekday(parse_date("2/4/1995")));  // Sunday
    EXPECT_TRUE  (is_weekday(parse_date("15/1/2021"))); // Friday
    EXPECT_FALSE (is_weekday(parse_date("16/1/2021"))); // Saturday

    EXPECT_TRUE  (is_weekday(parse_date("28/2/2020"))); // Friday
    EXPECT_FALSE (is_weekday(parse_date("29/2/2020"))); // Saturday
}

TEST(DateUtilsTest, is_same_day)
{
    EXPECT_TRUE  ( is_same_day(parse_date("15/1/2021"), parse_date("15/1/2021")));
    EXPECT_FALSE ( is_same_day(parse_date("1/1/2021"), parse_date("1/1/1999")));
}

TEST(DateUtilsTest, is_same_month)
{
    EXPECT_TRUE ( is_same_month(parse_date("15/1/2021"), parse_date("31/1/2021")));

    EXPECT_FALSE ( is_same_month(parse_date("15/1/2021"), parse_date("15/1/2020")));
}

TEST(DateUtilsTest, elapsed_days_without_weekends)
{
    EXPECT_EQ (8, elapsed_days_without_weekends(parse_date("8/7/2021"), parse_date("20/7/2021")));

    // Inverted order should return 0
    EXPECT_EQ (0, elapsed_days_without_weekends(parse_date("8/7/2021"), parse_date("7/7/2021")));

    // Different months in a leap year
    EXPECT_EQ (5, elapsed_days_without_weekends(parse_date("27/2/2020"), parse_date("5/3/2020")));

    // Different years
    EXPECT_EQ (261, elapsed_days_without_weekends(parse_date("8/7/2021"), parse_date("8/7/2022")));
}

TEST(DateUtilsTest, get_next_day)
{
    EXPECT_EQ(1, get_next_day(parse_date("31/12/2021")).tm_mday);
    EXPECT_EQ(0, get_next_day(parse_date("31/12/2021")).tm_mon);
    EXPECT_EQ(9, get_next_day(parse_date("8/7/2021")).tm_mday);
    EXPECT_EQ(1, get_next_day(parse_date("28/2/2021")).tm_mday);
    EXPECT_EQ(29, get_next_day(parse_date("28/2/2020")).tm_mday); // Leap year
}

TEST(DateUtilsTest, beginning_next_month)
{
    EXPECT_EQ(1, beginning_next_month(parse_date("15/1/2021")).tm_mday);
    EXPECT_EQ(1, beginning_next_month(parse_date("15/1/2021")).tm_mday); // 1 = February (zero-based)
    EXPECT_EQ(1, beginning_next_month(parse_date("31/1/2021")).tm_mday);
    EXPECT_EQ(1, beginning_next_month(parse_date("1/1/2022")).tm_mday);
    EXPECT_EQ(3, beginning_next_month(parse_date("1/3/2022")).tm_mon); // 3 = April (zero-based)
}

TEST(DateUtilsTest, compare_months)
{
    EXPECT_EQ(0, compare_months(parse_date("15/1/2021"), parse_date("31/1/2021"))); // same month
    EXPECT_EQ(14, compare_months(parse_date("15/1/2020"), parse_date("1/3/2021"))); // 14 months before
    EXPECT_EQ(6, compare_months(parse_date("15/7/2021"), parse_date("1/1/2022"))); // 6 months before
    EXPECT_EQ(-20, compare_months(parse_date("30/7/2021"), parse_date("15/11/2019"))); // 20 months later
    EXPECT_EQ(-20, compare_months(parse_date("1/10/2021"), parse_date("29/2/2020"))); // 20 months later
    EXPECT_EQ(-6, compare_months(parse_date("1/2/2022"), parse_date("15/8/2021"))); // 6 months later
}
