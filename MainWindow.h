#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QMainWindow>
#include <QScopedPointer>

#include "RenderUtilities.h"
#include "Scene.h"

namespace Ui {
class MainWindow;
}
class QProgressBar;
class QThread;
class Polygon;
class SpotLight;
class Renderer;

class MainWindow: public    QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    enum RendererType {RendSimple, RendParallel, RendTypesCount};

    QScopedPointer<Ui::MainWindow>  _ui;
    QScopedPointer<QProgressBar>    _progressBar = nullptr;

    QScopedPointer<QThread>         _workThread;
    QElapsedTimer                   _timer;
    bool                            _renderingIsRunning = false;

    Scene                           _scene;
    QScopedPointer<Renderer>        _renderer;

    void readSettings();
    void writeSettings();

    void setCanvas(const Canvas &canvas);
    void setRenderer(RendererType type);

private slots:
    void newFile();
    void saveScene() const;
    void loadScene();

    void addPolygon();
    void addLight();
    void editScene();

    void handleRenderStart();
    void handleRenderFinish();
    void updateRenderProgress(float weight);

    void render();
};

#endif // MAINWINDOW_H
