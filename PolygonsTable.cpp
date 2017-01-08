#include "PolygonsTable.h"

#include <QBrush>
#include <QColor>

#include "Rgb.h"

PolygonsTable::PolygonsTable(const std::vector<Polygon> &initialPolygons,
                             QObject *parent)
    : QAbstractTableModel(parent)
    , _polygons(initialPolygons)
{
}

int PolygonsTable::rowCount(const QModelIndex &) const
{
    return int(_polygons.size());
}

int PolygonsTable::columnCount(const QModelIndex &) const
{
    return ColumnsCount;
}

QVariant PolygonsTable::data(const QModelIndex &index, int role) const
{
    const int row = index.row();
    if (row < 0 || _polygons.size() <= size_t(row))
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case Ks:
            return QVariant(_polygons[row].getKs());
            break;
        case Cpk:
            return QVariant(_polygons[row].cosPower());
            break;
        }
    }

    if (role ==  Qt::BackgroundRole || role == Qt::EditRole) {
        switch (index.column()) {
        case Ka:
            {
                const Rgb rgb = _polygons[row].kaColor();
                return QBrush(QColor::fromRgbF(rgb.red(),
                    rgb.green(), rgb.blue()));
            }
            break;
        case Kd:
            {
                const Rgb rgb = _polygons[row].kdColor();
                return QBrush(QColor::fromRgbF(rgb.red(),
                    rgb.green(), rgb.blue()));
            }
            break;
        }
    }

    return QVariant();
}

bool PolygonsTable::setData(const QModelIndex &index, const QVariant &value,
                            int role)
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QAbstractTableModel::setData(index, value, role);

    const int row = index.row();
    const int col = index.column();
    if (row < 0 || int(_polygons.size()) <= row)
        return false;
    if (col < 0 || ColumnsCount <= col)
        return false;

    Polygon &pol = _polygons[row];
    switch (col) {
    case Ka:
        if (value.canConvert<QColor>())
        {
            const QColor rgb = qvariant_cast<QColor>(value);
            const Rgb newRgb(rgb.redF(), rgb.greenF(), rgb.blueF());
            pol.setKaColor(newRgb);
            return true;
        }
        return false;
        break;
    case Kd:
        if (value.canConvert<QColor>())
        {
            const QColor rgb = qvariant_cast<QColor>(value);
            const Rgb newRgb(rgb.redF(), rgb.greenF(), rgb.blueF());
            pol.setKdColor(newRgb);
            return true;
        }
        return false;
        break;
    case Ks:
        if (!value.canConvert<float>())
            return false;
        pol.setKs(value.toFloat());
        return true;
        break;
    case Cpk:
        if (!value.canConvert<int>())
            return false;
        pol.setCosPower(value.toInt());
        return true;
        break;
    }

    return false;
}

QVariant PolygonsTable::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (orientation == Qt::Vertical)
        return section + 1;

    const char *names[] = {"Ka", "Kd", "Ks", "Степень косинуса"};
    static_assert(sizeof(names) / sizeof(*names) == ColumnsCount,
        "Column names count is not equal to columns count.");
    if (0 <= section && section < ColumnsCount) {
        return names[section];
    } else {
        return "";
    }
}

Qt::ItemFlags PolygonsTable::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
