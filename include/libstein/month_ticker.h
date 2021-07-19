#pragma once

#include <ctime>

namespace libstein
{
    class MonthTicker
    {
    private:
        int ending_month;
        int ending_year;
    public:
        int month;
        int year;

        MonthTicker(std::tm);
        bool valid();
        void next();
    };
}
