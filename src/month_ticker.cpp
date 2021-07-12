#include <chrono>
#include "libstein/month_ticker.h"

using namespace libstein;

MonthTicker::MonthTicker(std::tm start)
{
    this->month = start.tm_mon;
    this->year = start.tm_year;
    
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    auto parts = std::localtime(&now_c);

    this->ending_month = parts->tm_mon;
    this->ending_year = parts->tm_year;

    this->valid();
}

bool MonthTicker::valid()
{
    bool result = true;

    if (this->year > this->ending_year)
    {
        result = false;
    }
    else if (this->year == this->ending_year)
    {
        result = (this->month <= this->ending_month);
    }

    return result;
}

void MonthTicker::next()
{
    this->month++;
    if (this->month > 11)
    {
        this->month = 0;
        this->year++;
    }
}