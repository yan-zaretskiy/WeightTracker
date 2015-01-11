#include "weightdataprovider.h"

namespace weighttracker
{

WeightDataProvider::WeightDataProvider() { }

WeightDataProvider::~WeightDataProvider() { }

WeightDataProvider& WeightDataProvider::getInstance() {
    static WeightDataProvider instance;
    return instance;
}


}
