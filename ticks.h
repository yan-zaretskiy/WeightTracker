#ifndef TICKS_H
#define TICKS_H
#include <vector>
#include <QString>
#include <QDate>

namespace ticksCalculations {

extern std::vector<double> niceIntervals;
extern std::vector<double> intIntervals;
extern std::vector<double> int12Intervals;

struct NumTicksData
{
    double niceMin;
    double niceMax;
    double tickSpacing;
};

NumTicksData niceTicks(double minPoint,
                       double maxPoint,
                       int maxTicks,
                       std::vector<double>& intervals = niceIntervals,
                       int base = 10);


struct DateTicksData
{
    QString format;
    std::vector<QDate> dates;
};

DateTicksData niceTicks(QDate minDate, QDate maxDate, int maxTicks);

}

#endif // TICKS_H

