#ifndef POLYGONSTABLE_H
#define POLYGONSTABLE_H

#include <vector>
#include <QAbstractTableModel>

#include "Polygon.h"

class PolygonsTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    PolygonsTable(const std::vector<Polygon> &initialPolygons,
                  QObject *parent = nullptr);

    int rowCount(const QModelIndex & parent) const override;
    int columnCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    Qt::ItemFlags flags(const QModelIndex & index) const override;

    std::vector<Polygon> polygons() const;

private:
    enum Column {DiffusWeigh, ReflWeigh, ReflCoeff, CosPower, ColumnsCount};

    std::vector<Polygon>    _polygons;
};


inline std::vector<Polygon> PolygonsTable::polygons() const
{
    return _polygons;
}

#endif // POLYGONSTABLE_H
