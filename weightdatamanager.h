#ifndef WEIGHTDATAMANAGER_H
#define WEIGHTDATAMANAGER_H

// This class manages the weight data. It is a wrapper around a vector of DataPoint structs
// that offers the following additional functionality:
// 1. When adding a data point it sorts the vector if necessary.
// 2. It can check whether a DataPoint with a certain date is present in the vector and
//    if true, it will give the position of that DataPoint.
//
// The intended scenario for using this class is the following:
// 1. When a new DataPoint needs to be added to the container, you first call
//    the hasDate() function.
// 2. If it returns false, then call addDataPoint().
// 3. If it returns true, then you can do something about it, like inform a user about the situation.
//    If you decide to go forward with a new value for the existing date, then call modifyValueAt()
//    using the previously obtained position.


#include <vector>
#include <utility>
#include <QDate>
#include "datapoint.h"

namespace weighttracker {

class WeightDataManager
{
public:
    WeightDataManager();
    ~WeightDataManager() = default;

    const std::vector<DataPoint>& getData() const;
    const DataPoint& at(size_t i) const;
    void clear();
    int dataSize() const;
    std::pair<bool,int> hasDate(const QDate& date) const;
    void addDataPoint(QDate date, double weight);
    void modifyValueAt(int i, double newValue);
    void removeDataPoints(int first, int count);

private:
    std::vector<DataPoint> data_;
};

} // namespace weighttracker

#endif // WEIGHTDATAMANAGER_H
