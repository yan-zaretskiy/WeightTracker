#ifndef WTWIDGET_H
#define WTWIDGET_H

#include <QWidget>
#include <QDate>

namespace Ui {
class WtWidget;
}

class QUndoStack;
class QCustomPlot;

namespace weighttracker {

class WeightTableModel;
class AddDataDialog;

class WtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WtWidget(QCustomPlot* plot, QWidget *parent = 0);
    ~WtWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void clearModel();
    QUndoStack* undoStack() const;

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void requestTrendsUpdate();
    void removeSelectedRows();
    void invokeAddDataDialog();
    void addRow(QDate date, double weight);

private:
    void initializePlot();
    void updatePlot();

    Ui::WtWidget* ui;

    WeightTableModel* model_;
    AddDataDialog* dialog_;
    QUndoStack* undoStack_;
    QCustomPlot* plot_;
};

} // namespace weighttracker

#endif // WTWIDGET_H
