#include "Delegate.h"

#include <QColorDialog>
#include <QDoubleSpinBox>

Delegate::Delegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , _commonDblRange({0., 1.})
    , _commonDblStep(0.01)
{
}

void Delegate::setDoubleRange(double min, double max)
{
    _commonDblRange.min = min;
    _commonDblRange.max = max;
}

void Delegate::setColumnDoubleRange(int col, double min, double max)
{
    auto iter = _doubleParams.find(col);
    if (iter == _doubleParams.end()) {
        iter = _doubleParams.insert(col, DoubleRange());
    }
    iter->min = min;
    iter->max = max;
}

void Delegate::setDoubleStep(double step)
{
    _commonDblStep = step;
}

QWidget * Delegate::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    const QVariant data = index.data(Qt::EditRole);
    if (data.canConvert<QColor>()) {
        return new QColorDialog(qvariant_cast<QColor>(data), parent);
    } else if (data.canConvert<QBrush>()) {
        return new QColorDialog(qvariant_cast<QBrush>(data).color(), parent);
    } else if (data.type() == QMetaType::Float ||
            data.type() == QMetaType::Double) {
        return doubleEditor(parent, index);
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QColorDialog *colorDialog = qobject_cast<QColorDialog*>(editor);
    if (colorDialog != nullptr) {
        const QVariant data = index.data(Qt::EditRole);
        QColor color;
        if (data.canConvert<QBrush>()) {
            color = qvariant_cast<QBrush>(data).color();
        } else if (data.canConvert<QColor>()) {
            color = qvariant_cast<QColor>(data);
        }

        colorDialog->setCurrentColor(color);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                            const QModelIndex &index) const
{
    QColorDialog *colorDialog = qobject_cast<QColorDialog*>(editor);
    if (colorDialog != nullptr) {
        model->setData(index, colorDialog->currentColor(), Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

QDoubleSpinBox *Delegate::doubleEditor(QWidget *parent, const QModelIndex &index) const
{
    QDoubleSpinBox *spin = new QDoubleSpinBox(parent);

    const auto iter = _doubleParams.find(index.column());
    if (iter == _doubleParams.end()) {
        spin->setMinimum(_commonDblRange.min);
        spin->setMaximum(_commonDblRange.max);
    } else {
        spin->setMinimum(iter->min);
        spin->setMaximum(iter->max);
    }
    spin->setSingleStep(_commonDblStep);
    spin->setValue(index.data(Qt::EditRole).toFloat());

    return spin;
}
