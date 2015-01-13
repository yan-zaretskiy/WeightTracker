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
    explicit WeightPlotManager(QCustomPlot* plot, QObject *parent = 0);
    ~WeightPlotManager();

signals:

public slots:
    void zoomIn();
    void zoomOut();
    void setupAxes();
    void initializePlot();
    void alterPoint(int pos, TableChange change);
    void updateTrends(double shift);
    void setRange(PlotRange range);

private:
    void setDateRange();
    void setWeightRange();

    QCustomPlot* plot_;
    QCPDataMap* weightData_;
    QCPDataMap* trendData_;
    PlotRange range_;

    WeightDataManager* wdm_;
    WeightDataAnalyzer* wda_;
};

}

#endif // WEIGHTPLOTMANAGER_H
