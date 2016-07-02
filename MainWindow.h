#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>

#include "RenderUtilities.h"
#include "Scene.h"

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

    Scene                           _scene;

    void setCanvas(const Canvas &canvas);

private slots:
    void newFile();
    void saveScene() const;
    void loadScene();

    void addPolygon();
    void addLight();

    void render();
};

#endif // MAINWINDOW_H
