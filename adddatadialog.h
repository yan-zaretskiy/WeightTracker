#ifndef ADDDATADIALOG_H
#define ADDDATADIALOG_H

#include <QDialog>
#include <QDate>

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

public slots:
    void accept();

signals:
    void requestDataInput(QDate date, double weight);

private slots:
    void on_weightEdit_textChanged();

private:
    Ui::AddDataDialog *ui;
};

}

#endif // ADDDATADIALOG_H
