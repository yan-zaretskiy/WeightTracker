#ifndef WEIGHTPLOTMANAGER_H
#define WEIGHTPLOTMANAGER_H

#include <QObject>

class QCustomPlot;

namespace weighttracker
{

class WeightPlotManager : public QObject
{
    Q_OBJECT
public:
    explicit WeightPlotManager(QCustomPlot* plot, QObject *parent = 0);
    ~WeightPlotManager();

signals:

public slots:

private:
    QCustomPlot* plot_;
};

}

#endif // WEIGHTPLOTMANAGER_H
