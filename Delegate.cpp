#include "Delegate.h"

#include <QColorDialog>

Delegate::Delegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
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
