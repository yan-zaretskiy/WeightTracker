#include <cmath>

#include "weightdataanalyzer.h"

namespace weighttracker {

using std::vector;

double WeightDataAnalyzer::tau() const
{
    return tau_;
}


void WeightDataAnalyzer::setTau(double tau)
{
    tau_ = tau;
}


double WeightDataAnalyzer::gamma() const
{
    return gamma_;
}


void WeightDataAnalyzer::setGamma(double gamma)
{
    gamma_ = gamma;
}


void WeightDataAnalyzer::computeTrendValues(const DataVector &weightData, int position)
{
    if (weightData.size() == 0)
    {
        trend_.clear();
        return;
    }

    trend_.resize(weightData.size());

    if (position == 0)
    {
        trend_[0] = weightData[0].value;
        ++position;
    }

    for(size_t i = position; i < weightData.size(); ++i)
    {
        double dt = static_cast<double>(weightData[i-1].date.daysTo(weightData[i].date));
        double alpha = tau_ != 0.0 ? std::exp(-dt / tau_) : 0.0;
        double transitionVar = (weightData[i].value - trend_[i-1])
                             * (weightData[i].value - trend_[i-1]);
        double xi = (1.0 - alpha) / (1.0 - alpha * (1.0 - exp(gamma_ * transitionVar)));
        trend_[i] = trend_[i-1] + xi * (weightData[i].value - trend_[i-1]);
    }
}


} // namespace weighttracker
