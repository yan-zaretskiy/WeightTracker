#include <cmath>

#include "ticks.h"

using std::vector;

namespace ticksCalculations {

/****************************** Numerical Ticks *********************************/

vector<double> niceIntervals = {1.0, 2.0, 2.5, 3.0, 5.0, 10.0};
vector<double> intIntervals = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0};
vector<double> int12Intervals = {1.0, 2.0, 3.0, 4.0, 6.0, 12.0};
vector<double> wgtIntervals = {0.5, 1.0, 2.0, 5.0, 10.0};

template<typename T>
T log(T x, int base)
{
    return std::log(x) / std::log(base);
}

double niceCeil(double x, vector<double>& intervals, int base = 10);
double niceFloor(double x, vector<double>& intervals, int base = 10);

double niceCeil(double x, vector<double>& intervals, int base)
{
    if (x == 0) return x;
    if (x < 0) return niceFloor(-x, intervals, base) * -1;
    double z = pow(base, floor(log(x, base)));
    for (int i = 0; i < intervals.size() - 1; ++i)
        if (x <= intervals[i] * z) return intervals[i] * z;
    return intervals.back() * z;
}

double niceFloor(double x, vector<double>& intervals, int base)
{
    if (x == 0) return x;
    if (x < 0) return niceCeil(-x, intervals, base) * -1;
    double z = pow(base, ceil(log(x, base)) - 1.0);
    for (int i = intervals.size() - 1; i > 0 ; --i)
        if (x >= intervals[i] * z) return intervals[i] * z;
    return intervals.front() * z;
}

double niceRound(double x, vector<double>& intervals, int base = 10)
{
    if (x == 0) return x;
    double z = pow(base, ceil(log(x, base)) - 1.0);
    for (int i = 0; i < intervals.size() - 1; ++i)
    {
        double result = intervals[i] * z;
        double cutoff = 0.5 * (result + intervals[i+1] * z);
        if (x <= cutoff) return result;
    }
    return intervals.back() * z;
}

NumTicksData niceTicks(double minPoint, double maxPoint, int maxTicks, vector<double>& intervals, int base)
{
    double delta = maxPoint - minPoint;
    if (delta == 0.0)
    {
        minPoint = niceFloor(minPoint, intervals, base);
        maxPoint = niceCeil(maxPoint, intervals, base);
        delta = maxPoint - minPoint;
        if (delta == 0.0)
        {
            minPoint -= 0.5;
            maxPoint += 0.5;
            delta = maxPoint - minPoint;
        }
    }
    NumTicksData ticksData;
    ticksData.tickSpacing = niceRound(delta / (maxTicks - 1), intervals, base);
    ticksData.niceMin = floor(minPoint / ticksData.tickSpacing) * ticksData.tickSpacing;
    ticksData.niceMax = ceil(maxPoint / ticksData.tickSpacing) * ticksData.tickSpacing;

    return ticksData;
}


NumTicksData niceWeightTicks(double minPoint, double maxPoint, int maxTicks)
{
    return niceTicks(ceil(minPoint - 1.0), floor(maxPoint+ 1.0), maxTicks, wgtIntervals);
}

/********************************* Date Ticks ************************************/

DateTicksData niceYearTicks(int minYear, int maxYear, int maxTicks)
{
    NumTicksData ticks = niceTicks(minYear, maxYear, maxTicks, intIntervals);
    QDate currentDate(ticks.niceMin,1,1);
    QDate endDate(ticks.niceMax,1,1);
    int i = 1;
    DateTicksData dateTicks;
    while (currentDate <= endDate)
    {
        dateTicks.dates.push_back(currentDate);
        int nextYear = static_cast<int>(ticks.niceMin + i * ticks.tickSpacing);
        currentDate = QDate(nextYear, 1, 1);
        ++i;
    }
    dateTicks.format = "yyyy";
    return dateTicks;
}


DateTicksData niceMonthTicks(int minMonth, int maxMonth, int minYear, int maxYear, int maxTicks)
{
    NumTicksData ticks = niceTicks(minMonth, (maxYear - minYear) * 12 + maxMonth, maxTicks, int12Intervals, 12);
    int minTicksYear = static_cast<int>(minYear + ticks.niceMin / 12);
    int minTicksMonth = static_cast<int>(static_cast<int>(ticks.niceMin) % 12) + 1;
    int maxTicksYear = static_cast<int>(minYear + ticks.niceMax / 12);
    int maxTicksMonth = static_cast<int>(static_cast<int>(ticks.niceMax) % 12) + 1;
    QDate currentDate(minTicksYear, minTicksMonth, 1);
    QDate endDate(maxTicksYear, maxTicksMonth, 1);
    int i = 1;
    DateTicksData dateTicks;
    while (currentDate <= endDate)
    {
        dateTicks.dates.push_back(currentDate);
        int year = static_cast<int>(minTicksYear + (ticks.niceMin + i * ticks.tickSpacing) / 12);
        int month = static_cast<int>(static_cast<int>(ticks.niceMin + i * ticks.tickSpacing) % 12) + 1;
        currentDate = QDate(year, month, 1);
        ++i;
    }
    dateTicks.format = "MMM yyyy";
    return dateTicks;
}


DateTicksData niceDayTicks(QDate minDate, int deltaDays, int maxTicks)
{
    NumTicksData ticks = niceTicks(0, deltaDays, maxTicks, intIntervals);
    QDate currentDate = minDate.addDays(ticks.niceMin);
    QDate endDate = minDate.addDays(ticks.niceMax);
    int i = 1;
    DateTicksData dateTicks;
    while (currentDate <= endDate)
    {
        dateTicks.dates.push_back(currentDate);
        currentDate = minDate.addDays(i * ticks.tickSpacing);
        ++i;
    }
    dateTicks.format = "MMM d";
    return dateTicks;
}


DateTicksData niceTicks(QDate minDate, QDate maxDate, int maxTicks)
{
    // Try years
    int minYear = minDate.year();
    int maxYear = maxDate.year();
    if (maxDate != QDate(maxYear, 1, 1)) ++maxYear;

    if (maxYear - minYear >= maxTicks - 1)
        return niceYearTicks(minYear, maxYear, maxTicks);

    // Try months
    maxYear = maxDate.year(); // we revert back the year change
    int minMonth = minDate.month() - 1;
    int maxMonth = maxDate.month() - 1;
    if (maxDate != QDate(maxYear, maxDate.month(), 1))
    {
        if (maxMonth == 11)
        {
            maxMonth = 0;
            ++maxYear;
        }
        else
            ++maxMonth;
    }

    if ( (maxYear - minYear) * 12 + maxMonth - minMonth >= maxTicks - 1 )
        return niceMonthTicks(minMonth, maxMonth, minYear, maxYear, maxTicks);

    // Finally try days
    maxDate.addDays(1);
    int deltaDays = minDate.daysTo(maxDate);
    return niceDayTicks(minDate, deltaDays, maxTicks);
}

}
