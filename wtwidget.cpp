#include <QTableView>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>

#include "wtwidget.h"
#include "ui_wtwidget.h"
#include "weightdataanalyzer.h"
#include "weighttablemodel.h"
#include "adddatadialog.h"


namespace weighttracker {

WtWidget::WtWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WtWidget),
    wdm_(), wda_(), model_(nullptr), dialog_(nullptr)
{
    ui->setupUi(this);

    wda_.setTau(ui->tauSpinBox->value());
    wda_.setGamma(ui->gammaSpinBox->value());

    connect(ui->tauSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrends()));
    connect(ui->gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateTrends()));

    model_ = new WeightTableModel(wdm_, wda_);
    connect(model_, SIGNAL(dataModified()), this, SIGNAL(dataModified()));
    connect(ui->removeRowButton, &QPushButton::clicked, this, &WtWidget::removeSelectedRows);
    connect(ui->addRowButton, &QPushButton::clicked, this, &WtWidget::invokeAddDataDialog);

    ui->weightDataView->setModel(model_);
    ui->weightDataView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->weightDataView->horizontalHeader()->setHighlightSections(false);
    ui->weightDataView->resizeColumnsToContents();
    ui->weightDataView->verticalHeader()->setVisible(false);
}


WtWidget::~WtWidget()
{
    delete ui;
}


bool WtWidget::readFile(const QString &fileName)
{
    auto result = model_->readFile(fileName);
    ui->weightDataView->scrollToBottom();
    return result;
}


bool WtWidget::writeFile(const QString &fileName)
{
    return model_->writeFile(fileName);
}


void WtWidget::clear()
{
    model_->clear();
}


void WtWidget::updateTrends()
{
    model_->updateTrends(ui->tauSpinBox->value(), ui->gammaSpinBox->value());
}


void WtWidget::removeSelectedRows()
{
    QModelIndexList rows = ui->weightDataView->selectionModel()->selectedRows();
    if (rows.isEmpty()) return;

    int first = rows.first().row();
    int count = rows.size();

    int r = QMessageBox::question(this, tr("Weight Tracker"),
                    tr("Are you sure you want to remove %1 %2?").arg(count)
                                                                .arg(count > 1 ? "rows" : "row"),
                    QMessageBox::Yes | QMessageBox::No);

    if (r == QMessageBox::Yes)
        model_->removeRows(first, count);
    else
        ui->weightDataView->setFocus();
}


void WtWidget::invokeAddDataDialog()
{
    if(!dialog_)
    {
        dialog_ = new AddDataDialog(this);
        dialog_->setModal(true);
        connect(dialog_, &AddDataDialog::requestDataInput, this, &WtWidget::possiblyAddRow);
    }
    dialog_->clearInput();
    dialog_->setFocusOnDateEdit();
    dialog_->show();
}


void WtWidget::possiblyAddRow(QDate date, double weight)
{
    auto result = wdm_.hasDate(date);
    int position = result.second;
    if(result.first)
    {
        int r = QMessageBox::warning(this, tr("Weight Tracker"),
                        tr("The record for %1 already exists.\n"
                           "Do you want to overwrite an old value\n"
                           "of %2 with a new value of %3?").arg(date.toString("MM/dd/yyyy"))
                                                           .arg(wdm_.at(position).value)
                                                           .arg(weight),
                        QMessageBox::Yes | QMessageBox::No);

        if (r == QMessageBox::No)
        {
            AddDataDialog* dialog = qobject_cast<AddDataDialog*>(sender());
            if (dialog)
            {
                dialog->show();
                dialog->setFocusOnDateEdit();
            }
            return;
        }
        model_->modifyWeightAtRow(position, weight);
    }
    else
    {
        model_->insertRowAt(position, date, weight);
        ui->weightDataView->scrollToBottom();
    }
}


} // namespace weighttracker