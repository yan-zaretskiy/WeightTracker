#ifndef UNDOCOMMANDS
#define UNDOCOMMANDS

#include <QUndoCommand>

namespace weighttracker {

class AddShapeCommand : public QUndoCommand
{
public:
    AddShapeCommand(Document *doc, const Shape &shape, QUndoCommand *parent = 0);
    void undo() Q_DECL_OVERRIDE;
    void redo() Q_DECL_OVERRIDE;

private:
    Document *m_doc;
    Shape m_shape;
    QString m_shapeName;
};

}

#endif // UNDOCOMMANDS

