#ifndef LIGHTTABLE_H
#define LIGHTTABLE_H

#include <vector>

#include <QAbstractTableModel>

#include "SpotLight.h"

class SpotLight;

class LightsTable: public   QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column {XCoord, YCoord, ZCoord, Intensivity, ColumnsCount};

    LightsTable(const std::vector<SpotLight> &initialLights,
               QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    std::vector<SpotLight> lights() const;

private:
    std::vector<SpotLight>  _lights;
};


inline std::vector<SpotLight> LightsTable::lights() const
{
    return _lights;
}

#endif // LIGHTTABLE_H
