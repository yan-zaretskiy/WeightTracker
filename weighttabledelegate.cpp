#include <QDateEdit>
#include <QCalendarWidget>

#include "weighttabledelegate.h"


namespace weighttracker {

WeightTableDelegate::WeightTableDelegate(QObject *parent)
    : QItemDelegate(parent) {}

QWidget *WeightTableDelegate::createEditor(QWidget *parent,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        QDateEdit *editor = new QDateEdit(parent);
        editor->setDisplayFormat("MM/dd/yyyy");
        editor->setCalendarPopup(true);
        editor->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
        editor->calendarWidget()->setGridVisible(true);
        connect(editor, &QDateEdit::editingFinished, this, &WeightTableDelegate::commitAndCloseEditor);
        return editor;
    }
    else
        return QItemDelegate::createEditor(parent, option, index);
}

void WeightTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
        if (index.column() == 0)
        {
            QDateEdit *dateEditor = qobject_cast<QDateEdit *>(editor);
            if (dateEditor)
            {
                dateEditor->setDate(QDate::fromString(
                                        index.model()->data(index, Qt::EditRole).toString(),
                                        "MM/dd/yyyy"));
            }
        }
        else
            return QItemDelegate::setEditorData(editor, index);
}

void WeightTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        // 1. call hasDate
        // 2. if false: store current row; remove it; add new row
        // 3. else: prompt for action
        // 4. if user wants to update the record, enable editing of weight at a new date
        // 5. else return to editing the current date cell
/*
    auto result = wdm_.hasDate(date);
    int position = result.second;
    if(result.first)
    {
        int r = QMessageBox::warning(this, tr("Weight Tracker"),
                        tr("The record for %1 already exists.\n"
                           "Do you want to overwrite an old value\n"
                           "of %2 with a new value of %3?").arg(date.toString("MM/dd/yyyy"))
                                                           .arg(wdm_.getData().at(position).value)
                                                           .arg(weight),
                        QMessageBox::Yes | QMessageBox::No);

        if (r == QMessageBox::No)
        {
            AddDataDialog* dialog = qobject_cast<AddDataDialog*>(sender());
            if (dialog) dialog->show();
            return;
        }
        model->modifyWeightAtRow(position, weight);
    }
    else
        model->insertRowAt(position, date, weight);
*/
    }
    else
        return QItemDelegate::setModelData(editor, model, index);
}

void WeightTableDelegate::commitAndCloseEditor()
{
    QDateEdit *editor = qobject_cast<QDateEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


}
