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
    RemoveRowCommand(QUndoCommand *parent = 0, WeightTableModel *model, int row);
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
    AddRowCommand(QUndoCommand *parent = 0, WeightTableModel *model, int row, DataPoint dp);
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
    ModifyRowCommand(QUndoCommand *parent = 0, WeightTableModel *model, int row, double weight);
    void undo() override;
    void redo() override;

private:
    WeightTableModel* model_;
    int row_;
    double weight_;
};

}

#endif // UNDOCOMMANDS

