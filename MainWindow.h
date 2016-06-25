#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QScopedPointer>

#include "RenderUtilities.h"

namespace Ui {
class MainWindow;
}
class Polygon;
class SpotLight;

class MainWindow: public    QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    QScopedPointer<Ui::MainWindow>  _ui;

    std::vector<Polygon>            _obj;
    std::vector<SpotLight>          _lights;

    void setCanvas(const Canvas &canvas);

private slots:
    void newFile();
    void saveScene();
    void loadScene();

    void addPolygon();

    void render();
};

#endif // MAINWINDOW_H
