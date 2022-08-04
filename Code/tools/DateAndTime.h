#pragma once
#include "project.h"


namespace tools
{


/**
 * brief:
 * tools for date and time,
 * 
 * note:
 * some types might need to formated to char or CE string
 * 
 * public:
 * - current_time_utc   : std::string
 * - YYYY               : std::string
 * - YYYYMMDD           : std::string
 * - hhmmss             : std::string
 * 
 * protected:
 * - YEAR               : int
 * - MONTH              : int
 * - DAY                : int
 * - HOUR               : int
 * - MINUTE             : int
 * - SECOND             : int
 * 
 */
class DateAndTime
{
private:
    time_t rawtime = time(0);
    tm *timeinfo = localtime(&rawtime);


protected:
    const int YEAR = [&]{
            auto year = 1900 + timeinfo->tm_year;
            return year;
        }();

    const int MONTH = [&]{
            auto month = timeinfo->tm_mon + 1;
            return month;
        }();

    const int DAY = [&]{
            auto day = timeinfo->tm_mday;
            return day;
        }();

    const int HOUR = [&]{
            auto hour = timeinfo->tm_hour;
            return hour;
        }();

    const int MINUTE = [&]{
            auto minute = timeinfo->tm_min;
            return minute;
        }();

    const int SECOND = [&]{
            auto second = timeinfo->tm_sec;
            return second;
        }();

public:
    // return current time utc format (2022-06-21 18:10:14)
    const std::string current_time_utc = []{
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
            return ss.str();
        }();


    // return YYYY
    const std::string YYYY = [&]
        {
            auto year = std::to_string(YEAR);
            return year;
        }();


    // return YYYYMMDD
    const std::string YYYYMMDD = [&]
        {
            auto
            year = YEAR,
            month = MONTH,
            day = DAY;

            auto
            year_str = std::to_string(year),
            month_str = std::to_string(month),
            day_str = std::to_string(day);

            auto
            year_fmt = [&]{ return year_str; }(),
            month_fmt = [&]{ return (month_str.size() < 2) ? "0" + month_str : month_str; }(),
            day_fmt = [&]{ return (day_str.size() < 2) ? "0" + day_str : day_str; }();

            auto result = year_fmt + month_fmt + day_fmt;
            return result;
        }();


    // return hh::mm:ss
    const std::string hhmmss = [&]
        {
            auto
            hour = HOUR,
            minute = MINUTE,
            second = SECOND;

            auto
            hour_str = std::to_string(hour),
            minute_str = std::to_string(minute),
            second_str = std::to_string(second);

            auto
            hour_fmt = [&]{ return (hour_str.size() < 2) ? "0" + hour_str : hour_str; }(),
            minute_fmt = [&]{ return (minute_str.size() < 2) ? "0" + minute_str : minute_str; }(),
            second_fmt = [&]{ return (second_str.size() < 2) ? "0" + second_str : second_str; }();

            auto result = hour_fmt +":"+ minute_fmt +":"+ second_fmt;
            return result;
        }();
};


} // namespace tools
