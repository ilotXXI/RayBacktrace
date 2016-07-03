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

PolygonsTable::~PolygonsTable()
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

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case Ks:
            return QVariant(_polygons[row].getKs());
            break;
        case Cpk:
            return QVariant(_polygons[row].cosPower());
            break;
        }
        break;

    case Qt::BackgroundRole:
        switch (index.column()) {
        case Ka:
            {
                const Rgb rgb = _polygons[row].KaColor();
                return QBrush(QColor::fromRgbF(rgb.red(), rgb.green(), rgb.blue()));
            }
            break;
        case Kd:
            {
                const Rgb rgb = _polygons[row].KdColor();
                return QBrush(QColor::fromRgbF(rgb.red(), rgb.green(), rgb.blue()));
            }
            break;
        }
        break;
    }

    return QVariant();
}
