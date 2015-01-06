#ifndef WEIGHTTABLEDELEGATE_H
#define WEIGHTTABLEDELEGATE_H

#include <QItemDelegate>

namespace weighttracker {

class WeightTableDelegate : public QItemDelegate
{
public:
    WeightTableDelegate(QObject *parent = 0);
    ~WeightTableDelegate() = default;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

signals:
    void requestDataInput(QDate date, double weight);

private slots:
    void commitAndCloseEditor();
};

}

#endif // WEIGHTTABLEDELEGATE_H
