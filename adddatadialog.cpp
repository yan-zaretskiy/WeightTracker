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
    QDialog(parent), lastDateEditSection_(QDateEdit::DaySection),
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
    ui->dateEdit->installEventFilter(this);

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


void AddDataDialog::setFocusOnWeightEdit()
{
    ui->weightEdit->setFocus();
}


void AddDataDialog::accept()
{
    QDialog::accept();

    emit requestDataInput(ui->dateEdit->date(), ui->weightEdit->text().toDouble());
}


bool AddDataDialog::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->dateEdit)
    {
        QKeyEvent *keyEvent;
        switch (event->type())
        {
        case QEvent::KeyPress:
            keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key())
            {
            case Qt::Key_Tab:
                focusNextChild();
                break;
            case Qt::Key_Backtab:
                focusPreviousChild();
                break;
            case Qt::Key_Left:
                selectPreviousSection();
                break;
            case Qt::Key_Right:
                selectNextSection();
                break;
            default:
                return QWidget::eventFilter(object, event);
            }
            break;
        case QEvent::FocusIn:
            ui->dateEdit->setSelectedSection(lastDateEditSection_);
            break;
        case QEvent::FocusOut:
            lastDateEditSection_ = ui->dateEdit->currentSection();
            ui->dateEdit->setSelectedSection(QDateEdit::NoSection);
            break;
        default:
            return QWidget::eventFilter(object, event);
        }
        return true;
    }
    return QWidget::eventFilter(object, event);
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


void AddDataDialog::selectPreviousSection()
{
    int currentSection = ui->dateEdit->currentSectionIndex();
    int nextSection = (currentSection == 0 ? ui->dateEdit->sectionCount()-1 : currentSection-1);
    ui->dateEdit->setSelectedSection(ui->dateEdit->sectionAt(nextSection));
}


void AddDataDialog::selectNextSection()
{
    int currentSection = ui->dateEdit->currentSectionIndex();
    int nextSection = (currentSection == ui->dateEdit->sectionCount()-1 ? 0 : currentSection+1);
    ui->dateEdit->setSelectedSection(ui->dateEdit->sectionAt(nextSection));
}


}
