#include <QTableView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QKeyEvent>
#include <QUndoStack>

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
    model_(nullptr), dialog_(nullptr), undoStack_(nullptr)
{
    ui->setupUi(this);
    WeightDataAnalyzer& wda = WeightDataProvider::getInstance().wdAnalyzer();
    WeightDataManager& wdm = WeightDataProvider::getInstance().wdManager();
    wda.setTau(ui->tauSpinBox->value());
    wda.setGamma(ui->gammaSpinBox->value());

    connect(ui->tauSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));
    connect(ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrend()));

    undoStack_ = new QUndoStack(this);
    connect(undoStack_, SIGNAL(cleanChanged(bool)), QWidget::window(), SLOT(weightTableModified()));

    connect(ui->removeRowButton, &QPushButton::clicked, this, &WtWidget::removeSelectedRows);
    connect(ui->addRowButton, &QPushButton::clicked, this, &WtWidget::invokeAddDataDialog);

    model_ = new WeightTableModel(wdm, wda, undoStack_);
    connect(model_, SIGNAL(rowModified(int)), this, SLOT(forwardRowModified(int)));
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
    emit trendUpdated(ui->shiftSpinBox->value());
}

void WtWidget::forwardRowModified(int row)
{
    emit weightAltered(row, TableChange::Modify);
}


void WtWidget::removeSelectedRows()
{
    QModelIndexList rows = ui->weightDataView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return;

    int first = rows.first().row();
    undoStack_->push(new RemoveRowCommand(model_, first));
    emit weightAltered(first, TableChange::Remove);
}


void WtWidget::invokeAddDataDialog()
{
    if(!dialog_)
    {
        dialog_ = new AddDataDialog(this);
        dialog_->setModal(true);
        connect(dialog_, &AddDataDialog::requestDataInput, this, &WtWidget::addRow);
    }
    dialog_->clearInput();
    dialog_->setFocusOnDateEdit();
    dialog_->show();
}


void WtWidget::addRow(QDate date, double weight)
{
    auto result = WeightDataProvider::getInstance().wdManager().hasDate(date);
    int position = result.second;

    undoStack_->push(new AddRowCommand(model_, position, date, weight));
    ui->weightDataView->scrollToBottom();
    emit weightAltered(position, TableChange::Add);
}


} // namespace weighttracker
