#ifndef UNDOCOMMANDS
#define UNDOCOMMANDS

#include <QUndoCommand>
#include "datapoint.h"

namespace weighttracker {

class WeightTableModel;

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

}

#endif // UNDOCOMMANDS

