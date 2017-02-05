#ifndef ADDPOLYGONDIALOG_H
#define ADDPOLYGONDIALOG_H

#include <QDialog>

#include "Polygon.h"

namespace Ui {
class AddPolygonDialog;
}
class QColor;

class AddPolygonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPolygonDialog(QWidget *parent = 0);
    ~AddPolygonDialog();

    Polygon polygon() const;

public slots:
    void accept() override;

private:
    Ui::AddPolygonDialog    *_ui;
    void setupDefaultData();

    std::vector<Point> readVertices() const;
    std::pair<Rgb, Rgb> readRgbs() const;

    bool canRemove() const;

    static void setColor(QPushButton *toButton, const QColor &color);
    static QColor color(QPushButton *ofButton);
    void askAndSetColor(QPushButton *forButton);

private slots:
    void addVertice();
    void removeVertice();

    void updateRemoveEnabled();
};

#endif // ADDPOLYGONDIALOG_H
