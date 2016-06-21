#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>

#include <QMainWindow>
#include <QScopedPointer>

#include "RenderUtilities.h"

namespace Ui {
class MainWindow;
}
class polygon;
class spotLight;

class MainWindow: public    QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

private:
    QScopedPointer<Ui::MainWindow>  _ui;

    std::vector<polygon>            _obj;
    std::vector<spotLight>          _lights;

    void setCanvas(const Canvas &canvas);

private slots:
    void newFile();
    void saveScene();
    void loadScene();

    void render();
};

#endif // MAINWINDOW_H
