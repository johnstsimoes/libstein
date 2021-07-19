#include <string>
#include <sstream>
#include <iomanip>

#include "libstein/date_utils.h"

using namespace libstein::dateutils;

const std::tm libstein::dateutils::parse_date(const std::string& date)
{
    std::tm parsed;
    std::istringstream ss(date);
    ss >> std::get_time(&parsed, "%d/%m/%y");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    }
    return parsed;
}

bool libstein::dateutils::is_weekday(const std::tm &date)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
        date.tm_mday,
        date.tm_mon,
        date.tm_year};

  std::time_t time_temp = std::mktime(&time_in);
  const std::tm * time_out = std::localtime(&time_temp);

  return (     (time_out->tm_wday > 0)
            && (time_out->tm_wday < 6));
}

bool libstein::dateutils::is_same_day(const std::tm &day1, const std::tm &day2)
{
    return (    (day1.tm_mday == day2.tm_mday)
            &&  (day1.tm_mon == day2.tm_mon)
            &&  (day1.tm_year == day2.tm_year));
}

bool libstein::dateutils::is_same_month(const std::tm &date1, const std::tm &date2)
{
    return (    (date1.tm_year == date2.tm_year)
            &&  (date1.tm_mon  == date2.tm_mon));
}

int libstein::dateutils::elapsed_days_without_weekends(const std::tm &begin, const std::tm &end)
{
    // std::time_t elapsed_days = (std::mktime(&(active_block.end)) - std::mktime(&(active_block.begin))) / 86400;
    auto cursor = begin;
    int result = 0;

    while (!is_same_day(cursor, end))
    {
        if (is_weekday(cursor))
            result++;

        cursor = get_next_day(cursor);
    }

    return result;
}

std::tm libstein::dateutils::get_next_day(const std::tm &date)
{
  std::tm time_in = { 0, 0, 0, // second, minute, hour
        date.tm_mday,
        date.tm_mon,
        date.tm_year};

    std::time_t time_temp = std::mktime(&time_in);
    time_temp += 86400;
    std::tm * time_out = std::localtime(&time_temp);

    return *time_out;
}

std::tm libstein::dateutils::beginning_next_month(const std::tm& date)
{
    std::tm result = date;
    result.tm_mday = 1;
    result.tm_mon++;

    if (result.tm_mon >= 12)
    {
        result.tm_mon = 0;
        result.tm_year++;
    }

    return result;
}
