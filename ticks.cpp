#include <cmath>

#include "ticks.h"

namespace ticksCalculations {

double niceNum(double range, bool round)
{
    double exponent = floor(log10(range));
    double fraction = range / pow(10, exponent);
    double niceFraction;

    if (round) {
      if (fraction < 1.5)
        niceFraction = 1;
      else if (fraction < 3)
        niceFraction = 2;
      else if (fraction < 7)
        niceFraction = 5;
      else
        niceFraction = 10;
    } else {
      if (fraction <= 1)
        niceFraction = 1;
      else if (fraction <= 2)
        niceFraction = 2;
      else if (fraction <= 5)
        niceFraction = 5;
      else
        niceFraction = 10;
    }

    return niceFraction * pow(10, exponent);
}


TicksData niceTicks(double minPoint, double maxPoint, int maxTicks)
{
    minPoint = ceil(minPoint - 1.0);
    maxPoint = floor(maxPoint + 1.0);

    TicksData ticksData;
    ticksData.range = niceNum(maxPoint - minPoint, false);
    ticksData.tickSpacing = niceNum(ticksData.range / (maxTicks - 1), true);
    ticksData.niceMin = floor(minPoint / ticksData.tickSpacing) * ticksData.tickSpacing;
    ticksData.niceMax = ceil(maxPoint / ticksData.tickSpacing) * ticksData.tickSpacing;

    return ticksData;
}

}
