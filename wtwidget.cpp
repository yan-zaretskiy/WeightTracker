#include <QTableView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QUndoStack>
#include <QSettings>

#include "wtwidget.h"
#include "ui_wtwidget.h"
#include "weightdataprovider.h"
#include "weighttablemodel.h"
#include "weighttablemodelio.h"
#include "adddatadialog.h"
#include "undocommands.h"
#include "common.h"

namespace weighttracker {

WtWidget::WtWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WtWidget),
    model_(nullptr), undoStack_(nullptr)
{
    ui->setupUi(this);
    WeightDataAnalyzer& wda = WeightDataProvider::getInstance().wdAnalyzer();
    WeightDataManager& wdm = WeightDataProvider::getInstance().wdManager();
    wda.setTau(ui->tauSpinBox->value());
    wda.setGamma(ui->gammaSpinBox->value());

    connect(ui->tauSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));
    connect(ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));
    connect(ui->shiftSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(shiftChanged(int)));

    undoStack_ = new QUndoStack(this);
    connect(undoStack_, SIGNAL(cleanChanged(bool)), QWidget::window(), SLOT(weightTableModified()));

    connect(ui->removeRowButton, &QPushButton::clicked, this, &WtWidget::removeSelectedRows);
    connect(ui->addRowButton, &QPushButton::clicked, this, &WtWidget::invokeAddDataDialog);

    model_ = new WeightTableModel(wdm, wda, undoStack_);
    connect(model_, SIGNAL(rowModified(int)), this, SLOT(forwardRowModified(int)));
    connect(model_, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(forwardRowRemoved(QModelIndex,int)));
    connect(model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(forwardRowAdded(QModelIndex,int)));

    ui->weightDataView->setModel(model_);
    ui->weightDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->weightDataView->horizontalHeader()->setHighlightSections(false);
    ui->weightDataView->resizeColumnsToContents();
    ui->weightDataView->verticalHeader()->setVisible(false);
    ui->weightDataView->installEventFilter(this);
}


WtWidget::~WtWidget()
{
    delete ui;
}


bool WtWidget::readFile(const QString &fileName)
{
    bool result = WeightTableModelIO::populateModelFromFile(*model_, fileName);
    if (result)
    {
        ui->weightDataView->scrollToBottom();
        undoStack_->clear();
        emit dataReset();
    }
    return result;
}


bool WtWidget::writeFile(const QString &fileName)
{
    bool result = WeightTableModelIO::writeModelToFile(*model_, fileName);
    if (result) undoStack_->setClean();
    return result;
}


void WtWidget::clearModel()
{
    model_->clearWeightData();
    undoStack_->setClean();
    emit dataReset();
}


QUndoStack* WtWidget::undoStack() const
{
    return undoStack_;
}


bool WtWidget::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->weightDataView)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Delete)
            {
                removeSelectedRows();
                return true;
            }
            else return false;
        }
    }
    return QWidget::eventFilter(object, event);
}


void WtWidget::updateTrend()
{
    model_->updateTrends(ui->tauSpinBox->value(), ui->gammaSpinBox->value());
    emit trendUpdated();
}


void WtWidget::forwardRowModified(int row)
{
    emit weightAltered(row, TableChange::Modify);
}


void WtWidget::forwardRowRemoved(const QModelIndex &parent, int row)
{
    Q_UNUSED(parent)
    emit weightAltered(row, TableChange::Remove);
}


void WtWidget::forwardRowAdded(const QModelIndex &parent, int row)
{
    Q_UNUSED(parent)
    emit weightAltered(row, TableChange::Add);
}


void WtWidget::removeSelectedRows()
{
    QModelIndexList rows = ui->weightDataView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return;

    int first = rows.first().row();
    undoStack_->push(new RemoveRowCommand(model_, first));
}


void WtWidget::invokeAddDataDialog()
{
    AddDataDialog dialog(this);
    connect(&dialog, &AddDataDialog::requestDataInput, this, &WtWidget::addRow);
    dialog.setFocusOnWeightEdit();
    dialog.exec();
}


void WtWidget::addRow(QDate date, double weight)
{
    auto result = WeightDataProvider::getInstance().wdManager().hasDate(date);
    int position = result.second;

    undoStack_->push(new AddRowCommand(model_, position, date, weight));
    ui->weightDataView->scrollToBottom();
}


void WtWidget::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Yan Zaretskiy", "Weight Tracker");
    ui->tauSpinBox->setValue(settings.value("tau", 9.5).toDouble());
    ui->gammaSpinBox->setValue(settings.value("gamma", 0.0).toDouble());
    ui->shiftSpinBox->setValue(settings.value("shift", 0).toInt());
}


void WtWidget::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Yan Zaretskiy", "Weight Tracker");
    settings.setValue("tau", ui->tauSpinBox->value());
    settings.setValue("gamma", ui->gammaSpinBox->value());
    settings.setValue("shift", ui->shiftSpinBox->value());
}


} // namespace weighttracker
