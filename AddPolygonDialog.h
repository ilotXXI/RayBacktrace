#ifndef ADDPOLYGONDIALOG_H
#define ADDPOLYGONDIALOG_H

#include <QDialog>

#include "Polygon.h"

namespace Ui {
class AddPolygonDialog;
}
class QStandardItemModel;

class AddPolygonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPolygonDialog(QWidget *parent = 0);
    ~AddPolygonDialog();

private:
    Ui::AddPolygonDialog    *_ui;

    QStandardItemModel      *_coeffModel;

    static QStandardItemModel * createCoeffModel(QObject *parent);

private slots:
    void addVertice();
    void removeVertice();
};

#endif // ADDPOLYGONDIALOG_H
