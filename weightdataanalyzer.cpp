#include <cmath>
#include <dlib/optimization.h>

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
        int count = std::min(static_cast<int>(weightData.size()), 4);
        double temp = 0.0;
        for (int i = 0; i < count; ++i)
            temp += weightData[i].value;
        temp /= count;
        trend_[0] =  temp;
        ++position;
    }

    double dt, alpha, xi, error;
    for(size_t i = position; i < weightData.size(); ++i)
    {
        dt = static_cast<double>(weightData[i-1].date.daysTo(weightData[i].date));
        alpha = (tau_ != 0.0) ? std::exp(-dt / tau_) : 0.0;
        error = weightData[i].value - trend_[i-1];
        xi = (1.0 - alpha) / (1.0 - alpha * (1.0 - exp(-gamma_ * error * error)));
        trend_[i] = trend_[i-1] + xi * error;
    }
}


typedef dlib::matrix<double,0,1> FitParameters;

class Fitter
{
public:
    Fitter(const DataVector& data) : data_(data) {}
    double operator() (const FitParameters& params) const;

private:
    const DataVector& data_;
};


double Fitter::operator()(const FitParameters &params) const
{
    double tau = params(0);
    double gamma = params(1);

    double result = 0.0;
    double dt, alpha, xi, error;

    int count = std::min(static_cast<int>(data_.size()), 4);
    double trendValue = 0.0;
    for (int i = 0; i < count; ++i)
        trendValue += data_[i].value;
    trendValue /= count;

    for(size_t i = 1; i < data_.size(); ++i)
    {
        dt = static_cast<double>(data_[i-1].date.daysTo(data_[i].date));
        alpha = (tau != 0.0) ? std::exp(-dt / tau) : 0.0;
        error = data_[i].value - trendValue;
        result += error * error;
        xi = (1.0 - alpha) / (1.0 - alpha * (1.0 - exp(-gamma * error * error)));
        trendValue += xi * error;
    }
    return result;
}


std::pair<double, double> WeightDataAnalyzer::fitData(const DataVector &data)
{
    FitParameters params(2), lowerBounds(2), upperBounds(2);
    params = 9.5, 0.0;
    lowerBounds = 0.0, -10.0;
    upperBounds = 20.0, 10.0;
    Fitter fitter(data);
    dlib::find_min_bobyqa(fitter, params,
                          6,    // number of interpolation points
                          lowerBounds, upperBounds,   // lower/upper bound constraint
                          5,    // initial trust region radius
                          1e-6,  // stopping trust region radius
                          100    // max number of objective function evaluations
    );

    return std::make_pair(params(0), params(1));
}


} // namespace weighttracker
