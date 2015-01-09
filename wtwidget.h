#ifndef WTWIDGET_H
#define WTWIDGET_H

#include <QWidget>
#include "weightdatamanager.h"
#include "weightdataanalyzer.h"


namespace Ui {
class WtWidget;
}

namespace weighttracker {

class WeightTableModel;
class AddDataDialog;

class WtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WtWidget(QWidget *parent = 0);
    ~WtWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void clearModel();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void requestTrendsUpdate();
    void removeSelectedRows();
    void invokeAddDataDialog();
    void possiblyAddRow(QDate date, double weight);

private:
    Ui::WtWidget* ui;

    WeightDataManager wdm_;
    WeightDataAnalyzer wda_;
    WeightTableModel* model_;
    AddDataDialog* dialog_;

};

} // namespace weighttracker

#endif // WTWIDGET_H
