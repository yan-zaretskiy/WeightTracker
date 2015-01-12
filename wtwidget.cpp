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
#include "qcustomplot.h"

namespace weighttracker {

WtWidget::WtWidget(QCustomPlot* plot, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WtWidget),
    model_(nullptr), dialog_(nullptr), undoStack_(nullptr), plot_(plot)
{
    ui->setupUi(this);
    WeightDataAnalyzer& wda = WeightDataProvider::getInstance().wdAnalyzer();
    WeightDataManager& wdm = WeightDataProvider::getInstance().wdManager();
    wda.setTau(ui->tauSpinBox->value());
    wda.setGamma(ui->gammaSpinBox->value());

    connect(ui->tauSpinBox, SIGNAL(valueChanged(double)), this, SLOT(requestTrendsUpdate()));
    connect(ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(requestTrendsUpdate()));

    undoStack_ = new QUndoStack(this);
    connect(undoStack_, SIGNAL(cleanChanged(bool)), QWidget::window(), SLOT(weightTableModified()));

    connect(ui->removeRowButton, &QPushButton::clicked, this, &WtWidget::removeSelectedRows);
    connect(ui->addRowButton, &QPushButton::clicked, this, &WtWidget::invokeAddDataDialog);

    model_ = new WeightTableModel(wdm, wda, undoStack_);
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
        if (model_->rowCount(QModelIndex()) > 0)
            initializePlot();
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


void WtWidget::requestTrendsUpdate()
{
    model_->updateTrends(ui->tauSpinBox->value(), ui->gammaSpinBox->value());
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
}

void WtWidget::initializePlot()
{
    plot_->xAxis->setLabel("Date");
    plot_->yAxis->setLabel("Trend weigth");
    plot_->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    plot_->xAxis->setDateTimeFormat(QLocale().dateFormat(QLocale::ShortFormat));

    QDate firstDate = WeightDataProvider::getInstance().wdManager().getData().front().date;
    QDate lastDate = WeightDataProvider::getInstance().wdManager().getData().back().date;
    double firstDateSec = QDateTime(firstDate).toTime_t();
    double lastDateSec = QDateTime(lastDate).toTime_t();
    plot_->xAxis->setRange(firstDateSec, lastDateSec);
    plot_->yAxis->setRange(0, 100);

    plot_->replot();
}


void WtWidget::updatePlot()
{

    plot_->replot();
}


} // namespace weighttracker
