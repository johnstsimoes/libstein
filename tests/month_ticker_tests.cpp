#include <gtest/gtest.h>
#include <chrono>
#include "libstein/month_ticker.h"

using namespace libstein;

TEST(MonthTicker, basic_tests) {

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    auto parts = std::localtime(&now_c);

    int current_year = parts->tm_year;
    int current_month = parts->tm_mon;

    struct std::tm early_2019 = *parts;
    
    // create a tm for january 2019, and a MonthTicker with it
    early_2019.tm_mon = 0;      
    early_2019.tm_year = 119;

    MonthTicker mt(early_2019);
    
    EXPECT_EQ (mt.month, 0);    // This is Jan 2019
    EXPECT_EQ (mt.year, 119);

    EXPECT_EQ (mt.valid(), true);

    // Advance to Feb 2019
    mt.next();
    EXPECT_EQ (mt.valid(), true);
    EXPECT_EQ (mt.month, 1);    // This is Jan 2019
    EXPECT_EQ (mt.year, 119);

    // Advance to Jan 2020
    for (int i=0; i< 12; i++)
    {
        mt.next();
    }

    EXPECT_EQ (mt.valid(), true);
    EXPECT_EQ (mt.year, 120);
}

TEST(MonthTicker, new_year)
{
    std::tm tm;
    tm.tm_mon=11;
    tm.tm_year=99;

    MonthTicker mt(tm);
    
    EXPECT_EQ (true, mt.valid());
    mt.next();
    EXPECT_EQ (0, mt.month);
    EXPECT_EQ (100, mt.year);
}

TEST(MonthTicker, until_valid)
{
    std::time_t time_t_now = std::time(nullptr);
    std::tm* t_now = std::localtime(&time_t_now);
    std::tm one_year_ago = *t_now;
    one_year_ago.tm_year--; // 1 year ago

    int pos = 0;
    MonthTicker mt(one_year_ago);

    while (mt.valid())
    {
        mt.next();
        pos++;
    }

    EXPECT_EQ (13, pos);
    EXPECT_FALSE (mt.valid());
}
