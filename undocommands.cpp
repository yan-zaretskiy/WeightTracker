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

}
