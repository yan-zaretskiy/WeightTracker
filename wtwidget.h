#ifndef WTWIDGET_H
#define WTWIDGET_H

#include <QWidget>
#include <vector>
#include "weightdatamanager.h"
#include "weightdataanalyzer.h"


namespace Ui {
class WTWidget;
}

namespace weighttracker {

class WeightTableModel;
class AddDataDialog;

class WTWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WTWidget(QWidget *parent = 0);
    ~WTWidget();

    bool readFile(const QString& fileName);
    bool writeFile(const QString& fileName);
    void clear();

signals:
    void dataModified();

private slots:
    void updateTrends();
    void removeSelectedRows();
    void invokeAddDataDialog();
    void possiblyAddRow(QDate date, double weight);

private:
    Ui::WTWidget* ui;

    WeightDataManager wdm_;
    WeightDataAnalyzer wda_;
    WeightTableModel* model_;
    AddDataDialog* dialog_;

};

} // namespace weighttracker

#endif // WTWIDGET_H
