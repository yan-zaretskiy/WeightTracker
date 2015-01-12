#include "weightplotmanager.h"

namespace weighttracker
{

WeightPlotManager::WeightPlotManager(QCustomPlot *plot, QObject *parent)
    : QObject(parent), plot_(plot)
{

}

WeightPlotManager::~WeightPlotManager()
{ }

}
