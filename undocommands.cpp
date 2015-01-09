#include <QLocale>

#include <undocommands.h>
#include <weighttablemodel.h>

namespace weighttracker {

/******************************************************************************
                               RemoveRowCommand
******************************************************************************/

RemoveRowCommand::RemoveRowCommand(QUndoCommand* parent, WeightTableModel* model, int row)
: QUndoCommand(parent), model_(model), row_(row)
{
    dp_ = model->wdm_.at(row);
}

void RemoveRowCommand::undo()
{
    model_->beginInsertRows(QModelIndex(), row_, row_);
    model_->wdm_.addDataPoint(dp_.date, dp_.weight);
    model_->refreshTrendsStartingAtRow(row_);
    model_->endInsertRows();
}

void RemoveRowCommand::redo()
{
    model_->beginRemoveRows(QModelIndex(), row_, row_);
    model_->wdm_.removeDataPoints(row_, 1);
    model_->refreshTrendsStartingAtRow(row_);
    model_->endRemoveRows();
}

/******************************************************************************
                                  AddRowCommand
******************************************************************************/

AddRowCommand::AddRowCommand(QUndoCommand* parent, WeightTableModel* model, int row, DataPoint dp)
: QUndoCommand(parent), model_(model), row_(row), dp_(dp)
{ }

void AddRowCommand::undo()
{
    model_->beginRemoveRows(QModelIndex(), row_, row_);
    model_->wdm_.removeDataPoints(row_, 1);
    model_->refreshTrendsStartingAtRow(row_);
    model_->endRemoveRows();
}

void AddRowCommand::redo()
{
    model_->beginInsertRows(QModelIndex(), row_, row_);
    model_->wdm_.addDataPoint(dp_.date, dp_.weight);
    model_->refreshTrendsStartingAtRow(row_);
    model_->endInsertRows();
}

/******************************************************************************
                               ModifyRowCommand
******************************************************************************/

ModifyRowCommand::ModifyRowCommand(QUndoCommand* parent, WeightTableModel* model, int row, double weight)
: QUndoCommand(parent), model_(model), row_(row), weight_(weight)
{ }

void ModifyRowCommand::undo()
{
    model_->beginInsertRows(QModelIndex(), row_, row_);
    model_->wdm_.addDataPoint(dp_.date, dp_.weight);
    model_->refreshTrendsStartingAtRow(row_);
    model_->endInsertRows();
}

void ModifyRowCommand::redo()
{
    model_->wdm_.modifyValueAt(row_, weight_);
    model_->refreshTrendsStartingAtRow(row);

    emit dataChanged(index(row, 1), index(row, 1));
    emit dataChanged(index(row, 2), index(wdm_.dataSize()-1, 2));
}

}
