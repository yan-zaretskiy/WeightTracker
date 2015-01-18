#ifndef WTWIDGET_H
#define WTWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
class WtWidget;
}

class QUndoStack;

namespace weighttracker {

class WeightTableModel;
class AddDataDialog;
enum class TableChange;

class WtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WtWidget(QWidget *parent = 0);
    ~WtWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void clearModel();
    QUndoStack* undoStack() const;
    void writeSettings();
    void readSettings();

signals:
    void dataReset();
    void trendUpdated();
    void weightAltered(int pos, TableChange change);
    void shiftChanged(int shift);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void updateTrend();
    void forwardRowModified(int row);
    void forwardRowRemoved(const QModelIndex& parent,int row);
    void forwardRowAdded(const QModelIndex &parent, int row);
    void removeSelectedRows();
    void invokeAddDataDialog();
    void addRow(QDate date, double weight);

private:
    Ui::WtWidget* ui;

    WeightTableModel* model_;
    QUndoStack* undoStack_;
};

} // namespace weighttracker

#endif // WTWIDGET_H
