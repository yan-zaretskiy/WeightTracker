#include "weightplotmanager.h"
#include "weightdataprovider.h"

namespace weighttracker
{

WeightPlotManager::WeightPlotManager(QCustomPlot *plot, QObject *parent)
    : QObject(parent), plot_(plot)
{
    plot_->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    plot_->setMinimumSize(450, 350);

    range_ = PlotRange::Month;

    wdm_ = &WeightDataProvider::getInstance().wdManager();
    wda_ = &WeightDataProvider::getInstance().wdAnalyzer();

    setupAxes();
}

WeightPlotManager::~WeightPlotManager() { }


PlotRange& operator++(PlotRange &pr)
{
    pr = (pr == PlotRange::All) ? PlotRange::All : static_cast<PlotRange>(static_cast<int>(pr) + 1);
    return pr;
}


PlotRange& operator--(PlotRange &pr)
{
    pr = (pr == PlotRange::Week) ? PlotRange::Week : static_cast<PlotRange>(static_cast<int>(pr) - 1);
    return pr;
}


void WeightPlotManager::zoomIn()
{
    --range_;
    setDateRange();
    plot_->replot();
}

void WeightPlotManager::zoomOut()
{
    ++range_;
    setDateRange();
    plot_->replot();
}


void WeightPlotManager::setDateRange()
{
    QDate lastDate = wdm_->getData().empty()
                    ? QDate::currentDate()
                    : wdm_->getData().back().date;
    QDate firstDate;

    // this function should also update number of tick labels

    switch (range_)
    {
    case PlotRange::Week:
        firstDate = lastDate.addDays(-7);
        break;
    case PlotRange::Month:
        firstDate = lastDate.addMonths(-1);
        break;
    case PlotRange::ThreeMonths:
        firstDate = lastDate.addMonths(-3);
        break;
    case PlotRange::All:
        firstDate = wdm_->getData().front().date;
        break;
    }

    plot_->xAxis->setRange(QDateTime(firstDate).toTime_t(), QDateTime(lastDate).toTime_t());
}


void WeightPlotManager::setWeightRange()
{
    plot_->yAxis->setRange(50, 90);
}


void WeightPlotManager::setupAxes()
{
    // configure bottom axis to show date and time instead of number:
    plot_->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot_->xAxis->setDateTimeFormat("MMM dd");
    // set axis labels:
    plot_->xAxis->setLabel("Date");
    plot_->yAxis->setLabel("Weight");
    // set a fixed tick-step to one tick per 5 days:
    plot_->xAxis->setAutoTickStep(false);
    int fiveDaysInSeconds = 5*86400;
    plot_->xAxis->setTickStep(fiveDaysInSeconds);
    plot_->xAxis->setSubTickCount(4);
    // make top and right axes visible but without ticks and labels:
    plot_->xAxis2->setVisible(true);
    plot_->yAxis2->setVisible(true);
    plot_->xAxis2->setTicks(false);
    plot_->yAxis2->setTicks(false);
    plot_->xAxis2->setTickLabels(false);
    plot_->yAxis2->setTickLabels(false);

    // set meaningful default ranges
    setDateRange();
    setWeightRange();
}


void WeightPlotManager::initializePlot()
{

}


void WeightPlotManager::alterPoint(int pos, TableChange change)
{

}


void WeightPlotManager::updateTrends(double shift)
{

}


void WeightPlotManager::setRange(PlotRange range)
{
    range_ = range;
}

}
