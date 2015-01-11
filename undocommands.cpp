#include <QLocale>

#include "undocommands.h"
#include "weighttablemodel.h"

namespace weighttracker {

/******************************************************************************
                               RemoveRowCommand
******************************************************************************/

RemoveRowCommand::RemoveRowCommand(WeightTableModel* model, int row, QUndoCommand* parent)
: QUndoCommand(parent), model_(model), row_(row)
{
    setText("remove row\n");
    dp_ = model->getWeightData().at(row_);
}

void RemoveRowCommand::undo()
{
    model_->insertRowAt(row_, dp_.date, dp_.value);
}

void RemoveRowCommand::redo()
{
    model_->removeRow(row_);
}

/******************************************************************************
                                  AddRowCommand
******************************************************************************/

AddRowCommand::AddRowCommand(WeightTableModel* model, int row, QDate date, double weight, QUndoCommand* parent)
: QUndoCommand(parent), model_(model), row_(row), dp_(date, weight)
{
    setText("add row\n");
}

void AddRowCommand::undo()
{
    model_->removeRow(row_);
}

void AddRowCommand::redo()
{
    model_->insertRowAt(row_, dp_.date, dp_.value);
}

/******************************************************************************
                               ModifyRowCommand
******************************************************************************/

ModifyRowCommand::ModifyRowCommand(WeightTableModel* model, int row, double weight, QUndoCommand* parent)
: QUndoCommand(parent), model_(model), row_(row), newWeight_(weight)
{
    setText("modify weight\n");
    oldWeight_ = model->getWeightData().at(row_).value;
}

void ModifyRowCommand::undo()
{
    model_->modifyWeightAtRow(row_, oldWeight_);
}

void ModifyRowCommand::redo()
{
    model_->modifyWeightAtRow(row_, newWeight_);
}

}
