#include "weightplotmanager.h"
#include "weightdataprovider.h"

namespace weighttracker
{

WeightPlotManager::WeightPlotManager(QCustomPlot* plot, WeightDataManager& wdm, WeightDataAnalyzer& wda, QObject *parent)
    : QObject(parent), plot_(plot), wdm_(wdm), wda_(wda)
{
    plot_->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    plot_->setMinimumSize(450, 350);

    shift_ = 0;
//    range_ = PlotRange::Month;

    setupPlot();
}

WeightPlotManager::~WeightPlotManager() { }

void WeightPlotManager::setShift(int shift)
{
    shift_ = shift;
    if(trendData_ && !trendData_->empty())
        refreshTrend();
}


void WeightPlotManager::setupPlot()
{
    // configure bottom axis to show date and time instead of number:
    plot_->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot_->xAxis->setDateTimeFormat("MMM dd");
    // set axis labels:
    plot_->xAxis->setLabel("Date");
    plot_->yAxis->setLabel("Weight");
    // make top and right axes visible but without ticks and labels:
    plot_->xAxis2->setVisible(true);
    plot_->yAxis2->setVisible(true);
    plot_->xAxis2->setTicks(false);
    plot_->yAxis2->setTicks(false);
    plot_->xAxis2->setTickLabels(false);
    plot_->yAxis2->setTickLabels(false);

    weightGraph_ = plot_->addGraph();
    trendGraph_ = plot_->addGraph();
    weightData_ = weightGraph_->data();
    trendData_ = trendGraph_->data();

    QPen thickRedPen;
    thickRedPen.setColor(Qt::darkRed);
    thickRedPen.setWidth(4);
    QPen lightGrayPen;
    lightGrayPen.setColor(Qt::lightGray);
    lightGrayPen.setWidth(2);

    trendGraph_->setPen(thickRedPen);
    weightGraph_->setPen(lightGrayPen);

    adjustDateRange();
    adjustWeightRange();
}


void WeightPlotManager::refreshAll()
{
    weightData_->clear();
    trendData_->clear();
    for (int i = 0; i < wdm_.dataSize(); ++i)
    {
        double date = QDateTime(wdm_.at(i).date).toTime_t();
        double shiftedDate = QDateTime(wdm_.at(i).date.addDays(shift_)).toTime_t();
        double weight = wdm_.at(i).value;
        double trend = wda_.trend().at(i);
        weightData_->insert(weightData_->constEnd(), date, QCPData(date, weight));
        trendData_->insert(trendData_->constEnd(), shiftedDate, QCPData(shiftedDate, trend));
    }

    if (wdm_.dataSize() == 1)
        trendGraph_->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
    else
        trendGraph_->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, 4));

    adjustDateRange();
    adjustWeightRange();
    plot_->replot();
}


void WeightPlotManager::refreshTrend()
{
    trendData_->clear();

    for (int i = 0; i < wdm_.dataSize(); ++i)
    {
        double shiftedDate = QDateTime(wdm_.at(i).date.addDays(shift_)).toTime_t();
        double trend = wda_.trend().at(i);
        trendData_->insert(trendData_->constEnd(), shiftedDate, QCPData(shiftedDate, trend));
    }

    plot_->replot();
}


void WeightPlotManager::adjustDateRange()
{
    QDate lastDate, firstDate;

    if (wdm_.getData().empty())
    {
        lastDate = QDate::currentDate();
        firstDate = lastDate.addMonths(-1);
    }
    else
    {
        lastDate = wdm_.getData().back().date;
        firstDate = wdm_.getData().front().date;
    }

    double dayInSecs = 86400.0;
    plot_->xAxis->setRange(QDateTime(firstDate).toTime_t()-dayInSecs, QDateTime(lastDate).toTime_t()+dayInSecs);
    // set a fixed tick-step to one tick per 5 days:
//    plot_->xAxis->setAutoTickStep(false);
//    int fiveDaysInSeconds = 5*86400;
//    plot_->xAxis->setTickStep(fiveDaysInSeconds);
//    plot_->xAxis->setSubTickCount(4);

}


void WeightPlotManager::adjustWeightRange()
{
    if (wdm_.getData().empty())
        plot_->yAxis->setRange(50, 90);
    else
    {
        double minWeight = wdm_.at(0).value, maxWeight = wdm_.at(0).value;
        for (auto& w : wdm_.getData())
        {
            minWeight = std::min(minWeight, w.value);
            maxWeight = std::max(maxWeight, w.value);
        }
        plot_->yAxis->setRange(std::floor(minWeight), std::ceil(maxWeight));
    }
}


//void WeightPlotManager::setRange(PlotRange range)
//{
//    range_ = range;
//}


//PlotRange WeightPlotManager::getRange()
//{
//    return range_;
//}

//PlotRange& operator++(PlotRange &pr)
//{
//    pr = (pr == PlotRange::Year) ? PlotRange::Year : static_cast<PlotRange>(static_cast<int>(pr) + 1);
//    return pr;
//}


//PlotRange& operator--(PlotRange &pr)
//{
//    pr = (pr == PlotRange::Week) ? PlotRange::Week : static_cast<PlotRange>(static_cast<int>(pr) - 1);
//    return pr;
//}


//void WeightPlotManager::zoomIn()
//{
//    --range_;
//    adjustDateRange();
//    plot_->replot();
//}

//void WeightPlotManager::zoomOut()
//{
//    ++range_;
//    adjustDateRange();
//    plot_->replot();
//}

}
