#include <QLocale>

#include "weighttablemodel.h"
#include "weightdatamanager.h"
#include "weightdataanalyzer.h"

namespace weighttracker {

int WeightTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return wdm_.dataSize();
}


int WeightTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 3;
}


QVariant WeightTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    int row = index.row();
    int col = index.column();

    if (!index.isValid() || row >= wdm_.dataSize() || col > 2)
        return QVariant();

    switch(role)
    {
    case Qt::DisplayRole:
        switch (col)
        {
        case 0:
            return QLocale().toString(wdm_.at(row).date, QLocale::ShortFormat);
            break;
        case 1:
            return QString::number(wdm_.at(row).value, 'f', 1);
            break;
        case 2:
            return QString::number(wda_.trend().at(row), 'f', 1);
            break;
        }
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignRight + Qt::AlignVCenter;
        break;
    }

    return QVariant();
}


QVariant WeightTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case 0:
                return QString("Date");
            case 1:
                return QString("Weight");
            case 2:
                return QString("Trend");
            }
        }
    }
    return QVariant();
}


bool WeightTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        bool weightIsValid = false;
        double weight = value.toDouble(&weightIsValid);
        if (weightIsValid && weight >= 0.0 && weight <= 1000.0)
        {
            modifyWeightAtRow(index.row(), weight);
            return true;
        }
    }
    return false;
}


Qt::ItemFlags WeightTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled;

    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index);
    if (index.column() == 1)
        itemFlags |= Qt::ItemIsEditable;

    return itemFlags;
}


void WeightTableModel::modifyWeightAtRow(int row, double weight)
{
    wdm_.modifyValueAt(row, weight);
    refreshTrendsStartingAtRow(row);

    emit dataChanged(index(row, 1), index(row, 1));
    emit dataChanged(index(row, 2), index(wdm_.dataSize()-1, 2));
    emit dataModified();
}


void WeightTableModel::insertRowAt(int row, QDate date, double weight)
{
    beginInsertRows(QModelIndex(), row, row);
    wdm_.addDataPoint(date, weight);
    refreshTrendsStartingAtRow(row);
    endInsertRows();
    emit dataModified();
}


void WeightTableModel::updateTrends(double tau, double gamma)
{
    wda_.setTau(tau);
    wda_.setGamma(gamma);
    if (wdm_.dataSize() > 0)
    {
        refreshTrendsStartingAtRow(0);
        emit dataChanged(index(0,2), index(wdm_.dataSize()-1, 2));
    }
}


void WeightTableModel::clear()
{
    beginResetModel();
    wdm_.clear();
    refreshTrendsStartingAtRow(0);
    endResetModel();
}


void WeightTableModel::refreshTrendsStartingAtRow(int row)
{
    wda_.computeTrendValues(wdm_.getData(), row);
}


bool WeightTableModel::removeRows(int first, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    beginRemoveRows(QModelIndex(), first, first+count-1);
    wdm_.removeDataPoints(first, count);
    refreshTrendsStartingAtRow(first);
    endRemoveRows();
    emit dataModified();

    return true;
}

} // namespace weighttracker
