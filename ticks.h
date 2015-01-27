#ifndef TICKS_H
#define TICKS_H

namespace ticksCalculations {

struct TicksData
{
    double niceMin;
    double niceMax;
    double range;
    double tickSpacing;
};

TicksData niceTicks(double minPoint, double maxPoint, int maxTicks);

}

#endif // TICKS_H

