/*
 datetime_utils.cpp     MindForger thinking notebook

 Copyright (C) 2016-2023 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "datetime_utils.h"

using namespace std;

namespace m8r {

time_t datetimeNow()
{
    return time(nullptr);
}

/**
 * @brief Convert character string to datetime.
 * @param datetime    (struct tm*)calloc(1, sizeof(struct tm)) is expected for result creation.
 */
struct tm* datetimeFrom(const char* s, struct tm* datetime)
{
    strptime(s, "%Y-%m-%d %H:%M:%S", datetime);
    return datetime;
}

/**
 * @brief Convert datetime to character string.
 * @param result    char[50] or bigger is expected for result serialization.
 * @returns pointer to C-string with string date representation result or nullptr on error.
 */
char* datetimeTo(const struct tm* datetime, char* result)
{
    if(!datetime) {
        return nullptr;
    }

    if(datetime->tm_isdst) {
#ifndef _WIN32
        tm c {
            0, // sec
            0, // min
            0, // hour
            0, // month day
            0, // month (0 - 11)
            0, // year - 1900
            0, // day of week
            0, // days in year
            0, // is DST
            0, // seconds - UTC
            0  // TZ abbreviation
        }; // missing initializer required by older GCC versions 4.8.5 and older
#else
        tm c {
            0, // sec
            0, // min
            0, // hour
            0, // month day
            0, // month
            0, // year - 1900
            0, // day of week
            0, // days in year
            0  // is DST
        };
#endif
        memcpy(&c, datetime, sizeof(c));
        if(c.tm_hour) {
            c.tm_hour=datetime->tm_hour-1;
        } else {
            // time travel back in time may have impact on y/m/... > roundtrip required
            time_t backOneHour = mktime(&c) - 60*60;
#ifndef _WIN32
            localtime_r(&backOneHour, &c);
#else
            localtime_s(&c, &backOneHour);
#endif
        }
        strftime(result, sizeof(result)*100, "%Y-%m-%d %H:%M:%S", &c);
    } else {
        strftime(result, sizeof(result)*100, "%Y-%m-%d %H:%M:%S", datetime);
    }

    return result;
}

std::string datetimeToString(const time_t ts)
{
    char to[50];
    if(datetimeTo(localtime(&ts), to)) {
        return string{to};
    }
    return "";
}

time_t datetimeSeconds(struct tm* datetime)
{
    return mktime(datetime);
}

enum class Pretty
{
    TODAY,
    THIS_WEEK,
    THIS_YEAR,
    LONG_TIME_AGO
};

std::string datetimeToPrettyHtml(const time_t ts)
{
    return datetimeToPrettyHtml(&ts);
}

constexpr time_t SIX_DAYS = 60 * 60 * 24 * 6;
std::string datetimeToPrettyHtml(const time_t* seconds)
{
    time_t now;
    time(&now);

    tm tsS = *localtime(seconds);
    tm* nowS = localtime(&now);

    Pretty pretty = Pretty::LONG_TIME_AGO;

    if(tsS.tm_year==nowS->tm_year) {
        if(tsS.tm_mon==nowS->tm_mon && tsS.tm_mday==nowS->tm_mday) {
            pretty = Pretty::TODAY;
        } else {
            if(now-*seconds < SIX_DAYS && tsS.tm_wday<=nowS->tm_wday) {
                pretty = Pretty::THIS_WEEK;
            } else {
                pretty = Pretty::THIS_YEAR;
            }
        }
    } else {
        if(now-*seconds < SIX_DAYS) {
            pretty = Pretty::THIS_WEEK;
        }
    }

    const char *background;
    char text[50];
    switch(pretty) {
    case Pretty::LONG_TIME_AGO:
        background = "BBBBBB";
        strftime(text, sizeof(text), "%Y", &tsS);
        break;
    case Pretty::THIS_YEAR:
        background = "888888";
        strftime(text, sizeof(text), "%b %e", &tsS);
        break;
    case Pretty::THIS_WEEK:
        background= "555555";
        strftime(text, sizeof(text), "%A", &tsS);
        break;
    // IMPROVE this month
    case Pretty::TODAY:
        background = "000000";
        strftime(text, sizeof(text), "%R", &tsS);
        break;
    }

    // note that long timestamp is added to the HTML - it is convenient for parsing/sorting/processing
    std::string result;
    result =
            "<div title='" +
            to_stringl(*seconds) +
            "' style='background-color: #" +
            string(background) +
            "; color: #ffffff; text-align: center; white-space: pre;'>" + "&nbsp;&nbsp;" +
            string(text) +
            "&nbsp;&nbsp;" + "</div>";
    return result;
}

} // m8r namespace

