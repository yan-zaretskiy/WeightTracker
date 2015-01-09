#ifndef WEIGHTTABLEMODEL_H
#define WEIGHTTABLEMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "datapoint.h"

namespace weighttracker {

class WeightDataManager;
class WeightDataAnalyzer;
class WeightTableModelIO;

class WeightTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    WeightTableModel(WeightDataManager& wdm,
                     WeightDataAnalyzer& wda,
                     QObject* parent = 0)
    : QAbstractTableModel(parent), wdm_(wdm), wda_(wda) {}

    ~WeightTableModel() = default;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    bool removeRows(int first, int count, const QModelIndex &parent = QModelIndex()) override;

    void modifyWeightAtRow(int row, double weight);
    void insertRowAt(int row, QDate date, double weight);
    void updateTrends(double tau, double gamma);
    void setData(DataVector&& data);
    const DataVector& getData() const;
    void clearData();

signals:
    void dataModified();

private:
    void refreshTrendsStartingAtRow(int row);
    WeightDataManager& getWdManager();
    WeightDataManager& wdm_;
    WeightDataAnalyzer& wda_;
};

} // namespace weighttracker

#endif // WEIGHTTABLEMODEL_H
