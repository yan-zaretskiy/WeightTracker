#ifndef WEIGHTDATAPROVIDER_H
#define WEIGHTDATAPROVIDER_H

#include "weightdatamanager.h"
#include "weightdataanalyzer.h"

namespace weighttracker
{

class WeightDataProvider
{
public:
    ~WeightDataProvider();
    static WeightDataProvider& getInstance();

    WeightDataManager& wdManager() {return wdm_;}
    WeightDataAnalyzer& wdAnalyzer() {return wda_;}

private:
    WeightDataProvider();
    WeightDataProvider(const WeightDataProvider&);
    WeightDataProvider& operator=(const WeightDataProvider& rhs);

    WeightDataManager wdm_;
    WeightDataAnalyzer wda_;
};

}

#endif // WEIGHTDATAPROVIDER_H
