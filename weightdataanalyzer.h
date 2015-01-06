#ifndef WEIGHTDATAANALYZER_H
#define WEIGHTDATAANALYZER_H

#include <vector>
#include "datapoint.h"

namespace weighttracker {

class WeightDataAnalyzer
{
public:
    WeightDataAnalyzer() : trend_(0), tau_(0.0), gamma_(0.0) {}
    ~WeightDataAnalyzer() = default;

    void computeTrendValues(const std::vector<DataPoint>& weightData, int position);

    const std::vector<double>& trend() const { return trend_; }

    double tau() const;
    void setTau(double tau);

    double gamma() const;
    void setGamma(double gamma);

private:
    std::vector<double> trend_;
    double tau_;
    double gamma_;
};

}

#endif // WEIGHTDATAANALYSIS

