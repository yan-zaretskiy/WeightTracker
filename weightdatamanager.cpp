#include "weightdatamanager.h"
#include <algorithm>

namespace weighttracker {

WeightDataManager::WeightDataManager() : data_(0) { data_.reserve(10); }


const std::vector<DataPoint>& WeightDataManager::getData() const
{
    return data_;
}


const DataPoint& WeightDataManager::at(size_t i) const
{
    return data_.at(i);
}

void WeightDataManager::clear()
{
    data_.clear();
}


int WeightDataManager::dataSize() const
{
    return static_cast<int>(data_.size());
}


bool operator<(const DataPoint& a, const DataPoint& b)
{
    return a.date < b.date;
}


bool operator<(const DataPoint& a, const QDate& b)
{
    return a.date < b;
}


std::pair<bool, int> WeightDataManager::hasDate(const QDate &date) const
{
    auto positionPair = std::make_pair(false, -1);

    if (data_.empty() || date < data_.front().date)
        positionPair.second = 0;
    else if (date > data_.back().date)
        positionPair.second = dataSize();
    else
    {
        auto low = std::lower_bound(data_.begin(), data_.end(), date);
        positionPair.first = ((*low).date == date);
        positionPair.second = static_cast<int>(low - data_.begin());
    }

    return positionPair;
}


void WeightDataManager::addDataPoint(QDate date, double weight)
{
    // this function assumes that date is not in the container
    bool isSorted = data_.empty() || date > data_.back().date;

    data_.push_back(DataPoint(date, weight));

    if(!isSorted)
        std::sort(data_.begin(), data_.end());
}


void WeightDataManager::modifyValueAt(int i, double newValue)
{
    if(i < 0 || i >= static_cast<int>(data_.size())) return;
    data_.at(i).value = newValue;
}

void WeightDataManager::removeDataPoints(int first, int count)
{
    int size =  static_cast<int>(data_.size());
    if(first < 0 || count < 1 || first >= size || first + count - 1 >= size) return;
    data_.erase(data_.begin() + first, data_.begin() + first + count);
}


} // namespace weighttracker
