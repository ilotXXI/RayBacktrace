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
class Renderer;

class MainWindow: public    QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    QScopedPointer<Ui::MainWindow>  _ui;

    Scene                           _scene;
    QScopedPointer<Renderer>        _renderer;

    void setCanvas(const Canvas &canvas);

private slots:
    void newFile();
    void saveScene() const;
    void loadScene();

    void addPolygon();
    void addLight();
    void editScene();

    void render();
};

#endif // MAINWINDOW_H
