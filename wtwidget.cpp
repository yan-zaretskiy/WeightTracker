#include <QTableView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QUndoStack>
#include <QSettings>
#include <QTimer>

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

    undoStack_ = new QUndoStack(this);
    model_ = new WeightTableModel(wdm, wda, undoStack_);

    establishConnections();
    setupTableView();
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
}


void WtWidget::establishConnections()
{
    connect(ui->tauSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));
    connect(ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));
    connect(ui->shiftSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(shiftChanged(int)));
    connect(ui->removeRowButton, &QPushButton::clicked, this, &WtWidget::removeSelectedRows);
    connect(ui->addRowButton, &QPushButton::clicked, this, &WtWidget::invokeAddDataDialog);

    connect(undoStack_, SIGNAL(cleanChanged(bool)), QWidget::window(), SLOT(weightTableModified()));

    connect(model_, &WeightTableModel::rowModified, this,
            [this](int row){ emit weightAltered(row, TableChange::Modify); });
    connect(model_, &WeightTableModel::rowsRemoved, this,
            [this](const QModelIndex&, int first, int){ emit weightAltered(first, TableChange::Remove); });
    connect(model_, &WeightTableModel::rowsInserted, this,
            [this](const QModelIndex&, int first, int){ emit weightAltered(first, TableChange::Add); });
    connect(model_, &WeightTableModel::rowsInserted,
            [this](){ QTimer::singleShot(0, ui->weightDataView, &QTableView::scrollToBottom); });
}


void WtWidget::setupTableView()
{
    ui->weightDataView->setModel(model_);
    ui->weightDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->weightDataView->horizontalHeader()->setHighlightSections(false);
    ui->weightDataView->resizeColumnsToContents();
    ui->weightDataView->verticalHeader()->setVisible(false);
    ui->weightDataView->installEventFilter(this);
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
