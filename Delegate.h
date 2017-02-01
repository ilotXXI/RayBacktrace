#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Delegate(QObject *parent = nullptr);

    void setDoubleRange(double min, double max);
    void setDoubleStep(double step);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:
    double  _doubleMin = 0.;
    double  _doubleMax = 1.;
    double  _doubleStep = 0.01;
};

#endif // DELEGATE_H
