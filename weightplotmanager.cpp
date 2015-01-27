#include "weightplotmanager.h"
#include "weightdataprovider.h"
#include "ticks.h"

namespace weighttracker
{

WeightPlotManager::WeightPlotManager(QCustomPlot* plot, WeightDataManager& wdm, WeightDataAnalyzer& wda, QObject *parent)
    : QObject(parent), plot_(plot), wdm_(wdm), wda_(wda)
{
    plot_->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    plot_->setMinimumSize(450, 350);

    shift_ = 0;

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
    // add title layout element:
    // first we create and prepare a plot title layout element:
    QCPPlotTitle *title = new QCPPlotTitle(plot_);
    title->setText("Weight Evolution");
    title->setFont(QFont("sans", 12, QFont::Bold));
    plot_->plotLayout()->insertRow(0);
    plot_->plotLayout()->addElement(0, 0, title);
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
    weightGraph_->setName("Weight");
    trendGraph_ = plot_->addGraph();
    trendGraph_->setName("Trend");
    weightData_ = weightGraph_->data();
    trendData_ = trendGraph_->data();

    QPen thickRedPen;
    thickRedPen.setColor(Qt::darkRed);
    thickRedPen.setWidth(4);
    QPen lightGrayPen;
    lightGrayPen.setColor(Qt::darkGray);
    lightGrayPen.setWidth(2);

    trendGraph_->setPen(thickRedPen);
    weightGraph_->setPen(lightGrayPen);
    weightGraph_->setLineStyle(QCPGraph::lsNone);
    weightGraph_->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

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
    plot_->legend->setVisible(wdm_.dataSize() > 0);

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

        auto ticks = ticksCalculations::niceTicks(minWeight, maxWeight, 10);

        plot_->yAxis->setAutoTickStep(false);
        plot_->yAxis->setTickStep(ticks.tickSpacing);
        plot_->yAxis->setAutoSubTicks(false);
        plot_->yAxis->setSubTickCount(0);
        plot_->yAxis->setRange(ticks.niceMin, ticks.niceMax);
    }
}

}
