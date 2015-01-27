#ifndef WEIGHTPLOTMANAGER_H
#define WEIGHTPLOTMANAGER_H

#include <QObject>
#include "qcustomplot.h"
#include "common.h"

namespace weighttracker
{

class WeightDataManager;
class WeightDataAnalyzer;

class WeightPlotManager : public QObject
{
    Q_OBJECT
public:
    explicit WeightPlotManager(QCustomPlot* plot, WeightDataManager& wdm, WeightDataAnalyzer& wda, QObject *parent = 0);
    ~WeightPlotManager();

signals:

public slots:
    void setShift(int shift);
    void setupPlot();
    void refreshAll();
    void refreshTrend();

private:
    void adjustDateRange();
    void adjustWeightRange();

    QCustomPlot* plot_;
    QCPGraph* weightGraph_;
    QCPGraph* trendGraph_;
    QCPDataMap* weightData_;
    QCPDataMap* trendData_;

    WeightDataManager& wdm_;
    WeightDataAnalyzer& wda_;
    int shift_;
};

}

#endif // WEIGHTPLOTMANAGER_H
