#ifndef UNDOCOMMANDS
#define UNDOCOMMANDS

#include <QUndoCommand>
#include "datapoint.h"

namespace weighttracker {

class WeightTableModel;

/******************************************************************************
                               RemoveRowCommand
******************************************************************************/

class RemoveRowCommand : public QUndoCommand
{
public:
    RemoveRowCommand(WeightTableModel *model, int row, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    WeightTableModel* model_;
    int row_;
    DataPoint dp_;
};

/******************************************************************************
                                  AddRowCommand
******************************************************************************/

class AddRowCommand : public QUndoCommand
{
public:
    AddRowCommand(WeightTableModel *model, int row, QDate date, double weight, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    WeightTableModel* model_;
    int row_;
    DataPoint dp_;
};

/******************************************************************************
                               ModifyRowCommand
******************************************************************************/

class ModifyRowCommand : public QUndoCommand
{
public:
    ModifyRowCommand(WeightTableModel *model, int row, double weight, QUndoCommand *parent = 0);
    void undo() override;
    void redo() override;

private:
    WeightTableModel* model_;
    int row_;
    double oldWeight_, newWeight_;
};

}

#endif // UNDOCOMMANDS

