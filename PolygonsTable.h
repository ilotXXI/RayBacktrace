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
    ~PolygonsTable();

    virtual int rowCount(const QModelIndex & parent) const override;
    virtual int columnCount(const QModelIndex & parent) const override;

    virtual QVariant data(const QModelIndex & index, int role) const override;

private:
    enum Column {Ka, Kd, Ks, Cpk, ColumnsCount};

    std::vector<Polygon>    _polygons;
};

#endif // POLYGONSTABLE_H
