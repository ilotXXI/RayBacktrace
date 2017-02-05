#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QMap>

class QDoubleSpinBox;

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Delegate(QObject *parent = nullptr);

    void setDoubleRange(double min, double max);
    void setColumnDoubleRange(int col, double min, double max);
    void setDoubleStep(double step);

    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

private:
    struct DoubleRange
    {
        double  min;
        double  max;
    };

    DoubleRange             _commonDblRange;
    double                  _commonDblStep;
    QMap<int, DoubleRange>  _doubleParams;

    QDoubleSpinBox * doubleEditor(QWidget *parent,
        const QModelIndex &index) const;
};

#endif // DELEGATE_H
