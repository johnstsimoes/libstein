#pragma once

#include <ctime>
#include <string>

namespace libstein
{
    namespace dateutils
    {
        const std::tm parse_date(const std::string& date);

        bool is_weekday(const std::tm &date);
        bool is_same_day(const std::tm &day1, const std::tm &day2);
        bool is_same_month(const std::tm &date1, const std::tm &date2);

        int elapsed_days_without_weekends(const std::tm &begin, const std::tm &end);
        std::tm get_next_day(const std::tm &date);
        std::tm beginning_next_month(const std::tm& date);
    }
}
