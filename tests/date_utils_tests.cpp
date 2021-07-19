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


        // bool same_day(const std::tm &day1, const std::tm &day2);
        // bool same_month(const std::tm &date1, const std::tm &date2);

        // int elapsed_days_without_weekends(const std::tm &begin, const std::tm &end);
        // std::tm get_next_day(const std::tm &date);
        // std::tm beginning_next_month(const std::tm& date);