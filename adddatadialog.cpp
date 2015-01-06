#include "adddatadialog.h"
#include "ui_adddatadialog.h"
#include <QCalendarWidget>
#include <QValidator>
#include <QPushButton>
#include <QLocale>

namespace weighttracker {

AddDataDialog::AddDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDataDialog)
{
    ui->setupUi(this);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(true);

    ui->dateEdit->setDisplayFormat(QLocale().dateFormat(QLocale::ShortFormat));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setFocus();
    ui->dateEdit->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
    ui->dateEdit->calendarWidget()->setGridVisible(true);

    ui->weightEdit->setValidator(new QDoubleValidator(0.0, 1000.0, 1));
}


AddDataDialog::~AddDataDialog()
{
    delete ui;
}


void AddDataDialog::clearInput()
{
    ui->weightEdit->clear();
    ui->dateEdit->setDate(QDate::currentDate());
}


void AddDataDialog::setFocusOnDateEdit()
{
    ui->dateEdit->setFocus();
}


void AddDataDialog::accept()
{
    QDialog::accept();

    emit requestDataInput(ui->dateEdit->date(), ui->weightEdit->text().toDouble());
}


void AddDataDialog::on_weightEdit_textChanged()
{
    bool isWeightOK = ui->weightEdit->hasAcceptableInput();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isWeightOK);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(isWeightOK);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(!isWeightOK);
}


}
