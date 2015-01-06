#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QDate>

namespace weighttracker {

struct DataPoint
{
    DataPoint() : date(QDate()), value(0.0) {}
    DataPoint(QDate qd) : date(qd), value(0.0) {}
    DataPoint(QDate qd, double w) : date(qd), value(w) {}
    QDate date;
    double value;
};

}

#endif // DATAPOINT_H

