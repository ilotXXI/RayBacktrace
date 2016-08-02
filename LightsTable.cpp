#include "LightsTable.h"

LightsTable::LightsTable(const std::vector<SpotLight> &initialLights, QObject *parent)
    : QAbstractTableModel(parent)
    , _lights(initialLights)
{
}

QVariant LightsTable::headerData(int section, Qt::Orientation orientation,
                                int role) const
{
    if (orientation == Qt::Vertical ||
            section < 0 || ColumnsCount <= section || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    const char *colNames[] = {"x", "y", "z", "Интенсивность"};
    return QString(colNames[section]);
}

int LightsTable::rowCount(const QModelIndex &) const
{
    return int(_lights.size());
}

int LightsTable::columnCount(const QModelIndex &) const
{
    return ColumnsCount;
}

QVariant LightsTable::data(const QModelIndex &index, int role) const
{
    const size_t row = size_t(index.row());
    if (!index.isValid() || _lights.size() <= row)
        return QVariant();
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    switch (index.column()) {
    case XCoord:
        return _lights[row].x();
        break;
    case YCoord:
        return _lights[row].y();
        break;
    case ZCoord:
        return _lights[row].z();
        break;
    case Intensivity:
        return _lights[row].Intensivity();
        break;
    }

    return QVariant();
}

bool LightsTable::setData(const QModelIndex &index, const QVariant &value,
                         int role)
{
    const size_t row = size_t(index.row());
    if (row < 0 || _lights.size() <= row)
        return false;
    if (data(index, role) == value)
        return false;
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return false;

    switch (index.column()) {
    case XCoord:
        _lights[row].setX(value.toFloat());
        break;
    case YCoord:
        _lights[row].setY(value.toFloat());
        break;
    case ZCoord:
        _lights[row].setZ(value.toFloat());
        break;
    case Intensivity:
        _lights[row].SetIntensivity(value.toFloat());
        break;
    default:
        return false;
        break;
    }

    emit dataChanged(index, index, QVector<int>() << role);
    return true;
}

Qt::ItemFlags LightsTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
