#include "adddatadialog.h"
#include "ui_adddatadialog.h"
#include <QCalendarWidget>
#include <QValidator>
#include <QPushButton>
#include <QLocale>
#include <QKeyEvent>
#include <QDebug>

#include "weightdataprovider.h"

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

    connect(ui->dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(updateButtons()));
    connect(ui->weightEdit, SIGNAL(textChanged(QString)), this, SLOT(updateButtons()));

    auto result = WeightDataProvider::getInstance().wdManager().hasDate(ui->dateEdit->date());
    ui->warningLabel->setVisible(result.first);
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

bool AddDataDialog::event(QEvent* event)
{
//    qDebug << static_cast<int>(event->type());
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab)
        {
            qDebug() << "kaka";//focusNextChild();
            return true;
        }
    }
    return QDialog::event(event);
}


void AddDataDialog::updateButtons()
{
    bool isInputOK = checkInput();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isInputOK);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setDefault(isInputOK);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setDefault(!isInputOK);
}


bool AddDataDialog::checkInput()
{
    bool isWeightOK = ui->weightEdit->hasAcceptableInput();
    auto result = WeightDataProvider::getInstance().wdManager().hasDate(ui->dateEdit->date());
    ui->warningLabel->setVisible(result.first);

    return !result.first && isWeightOK;
}


}
