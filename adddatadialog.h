#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>
#include <QDate>
#include <QDateTimeEdit>

namespace Ui {
class AddDataDialog;
}

namespace weighttracker {

class AddDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDataDialog(QWidget *parent = 0);
    ~AddDataDialog();

    void clearInput();
    void setFocusOnDateEdit();
    void setFocusOnWeightEdit();

public slots:
    void accept();

signals:
    void requestDataInput(QDate date, double weight);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void updateButtons();

private:
    bool checkInput();
    void selectPreviousSection();
    void selectNextSection();

    QDateTimeEdit::Section lastDateEditSection_;
    Ui::AddDataDialog *ui;
};

}

#endif // ADDDATADIALOG_H
