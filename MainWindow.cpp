#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTime>
#include <QThread>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QProgressBar>
#include <QCloseEvent>

#ifdef _DEBUG
#include <QDebug>
#endif

#include <memory>

#include "AddPolygonDialog.h"
#include "AddLightDialog.h"
#include "Polygon.h"
#include "SpotLight.h"
#include "EditSceneDialog.h"
#include "Canvas.h"
#include "SimpleRenderer.h"
#include "ParallelRenderer.h"

static const char *PATH_SETTING_NAME = "filePath";
static const char *GEOMETRY_SETTING_NAME = "geometry";
static const char *REND_SETTING_NAME = "renderer";

static inline std::unique_ptr<QSettings> settingsInst()
{
    QSettings *res = new QSettings(QSettings::IniFormat, QSettings::UserScope,
        QApplication::organizationName(), "RayBacktrace");
    res->beginGroup("RBMainWindow");
    return std::unique_ptr<QSettings>(res);
}

static inline QDataStream & operator <<(QDataStream &stream, const Rgb &rgb)
{
    return stream << rgb.red() << rgb.green() << rgb.blue();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
    , _progressBar(new QProgressBar(statusBar()))
    , _workThread(new QThread(this))
{
    _ui->setupUi(this);

    _progressBar->setMinimum(0);
    _progressBar->setMaximum(100);
    _progressBar->setValue(0);
    statusBar()->addPermanentWidget(_progressBar.data());
    _progressBar->setVisible(false);

    QActionGroup *rendChangeGroup = new QActionGroup(this);
    rendChangeGroup->setExclusive(true);
    rendChangeGroup->addAction(_ui->simpleRendAction);
    rendChangeGroup->addAction(_ui->parallelRendAction);

    connect(_ui->saveAction, &QAction::triggered, this, &MainWindow::saveScene);
    connect(_ui->loadAction, &QAction::triggered, this, &MainWindow::loadScene);
    connect(_ui->quitAction, &QAction::triggered, this, &MainWindow::close);
    connect(_ui->newFileAction, &QAction::triggered,
        this, &MainWindow::newFile);
    connect(_ui->renderAction, &QAction::triggered, this, &MainWindow::render);
    connect(_ui->addPolygonAction, &QAction::triggered,
        this, &MainWindow::addPolygon);
    connect(_ui->addLightAction, &QAction::triggered,
        this, &MainWindow::addLight);
    connect(_ui->editSceneAction, &QAction::triggered,
        this, &MainWindow::editScene);
    connect(_ui->simpleRendAction, &QAction::triggered,
        [this] (bool checked) {
            if (checked)
                setRenderer(RendSimple);
        });
    connect(_ui->parallelRendAction, &QAction::triggered,
        [this] (bool checked) {
            if (checked)
                setRenderer(RendParallel);
        });

    readSettings();

    _workThread->start();
}

MainWindow::~MainWindow()
{
    writeSettings();

    if (_workThread->isRunning()) {
        _workThread->quit();
        if (!_workThread->wait(5000)) {
            // There is a possible deadlock. Try to force
            // the thread termination.
            _workThread->terminate();
            _workThread->wait();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_renderingIsRunning) {
        QMessageBox::warning(this, tr("Закрытие окна"),
            tr("Невозможно закрыть окно во время рендеринга.\n"
               "Пожалуйста, дождитесь его окончания."));
        event->ignore();
    } else {
        QMainWindow::closeEvent(event);
    }
}

void MainWindow::readSettings()
{
    const std::unique_ptr<QSettings> settings = settingsInst();
    restoreGeometry(settings->value(GEOMETRY_SETTING_NAME).toByteArray());

    int rendType =
        settings->value(REND_SETTING_NAME, _defaultRendType).toInt();
    if (rendType < 0 || RendTypesCount <= rendType) {
        QMessageBox::warning(this, tr("Чтение настроек"),
            tr("При чтении настроек обнаружен неизвестный \n"
               "тип отрисовщика. Установлен отрисовщик по \n"
               "умолчанию."));
        rendType = _defaultRendType;
    }
    setRenderer(static_cast<RendererType>(rendType));
}

void MainWindow::writeSettings()
{
    const std::unique_ptr<QSettings> settings = settingsInst();

    settings->setValue(GEOMETRY_SETTING_NAME, saveGeometry());

    RendererType rendType = _defaultRendType;
    if (qobject_cast<SimpleRenderer *>(_renderer.data()) != nullptr) {
        rendType = RendSimple;
    } else if (qobject_cast<ParallelRenderer *>(_renderer.data()) != nullptr) {
        rendType = RendParallel;
    }
    settings->setValue(REND_SETTING_NAME, rendType);
}

void MainWindow::setCanvas(const Canvas &canvas)
{
    if (canvas.isEmpty()) {
        _ui->pixmapWidget->setPixmap(QPixmap());
        return;
    }

    auto rgbConvert = [](const Rgb &rgb) -> QRgb {
        return qRgb(rgb.red(), rgb.green(), rgb.blue());
    };

    const int width = canvas.width();
    const int height = canvas.height();
    QImage image(width, height, QImage::Format_RGB32);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            const QRgb color = rgbConvert(canvas.pixel(x, y));
            image.setPixel(x, y, color);
        }
    }

    _ui->pixmapWidget->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::setRenderer(RendererType type)
{
    switch (type) {
    case RendSimple:
        _renderer.reset(new SimpleRenderer);
        _ui->simpleRendAction->setChecked(true);
        break;
    case RendParallel:
        _renderer.reset(new ParallelRenderer);
        _ui->parallelRendAction->setChecked(true);
        break;
    default:
        Q_ASSERT(false);
        _renderer.reset(new SimpleRenderer);
        _ui->simpleRendAction->setChecked(true);
        break;
    }

    _renderer->moveToThread(_workThread.data());
    connect(_renderer.data(), &Renderer::renderStarted,
        this, &MainWindow::handleRenderStart);
    connect(_renderer.data(), &Renderer::renderFinished,
        this, &MainWindow::handleRenderFinish);
    connect(_renderer.data(), &Renderer::progressChanged,
        this, &MainWindow::updateRenderProgress);
}

void MainWindow::newFile()
{
    _scene.clear();
}

void MainWindow::saveScene() const
{
    std::unique_ptr<QSettings> settings = settingsInst();
    const QString initialPath = (settings.get() != nullptr) ?
        settings->value(PATH_SETTING_NAME).toString() : QString();

    const QString path =
        QFileDialog::getSaveFileName(const_cast<MainWindow *>(this),
        tr("Записать в файл..."), initialPath);
    if(path.isEmpty())
        return;

    //Открытие файла.
    QFile file(path);
    const bool isOpen = file.open(QIODevice::WriteOnly);
    if (!isOpen) {
        QMessageBox::warning(const_cast<MainWindow *>(this), tr("Запись сцены"),
            tr("Невозможно открыть файл\n\"%1\"").arg(path));
        return;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    //Запись в файл кол-в многоугольников и источников света.
    const std::vector<Polygon> &polygons = _scene.polygons();
    const std::vector<SpotLight> &lights = _scene.lights();
    stream << polygons.size() << lights.size();

    //Запись в файл многоугольников.
    for (const auto &polygon:    polygons)
    {
        const int verticesCount = polygon.verticesCount();

        //Запись коэффициентов и кол-ва вершин очередного многоугольника.
        stream << polygon.diffusionWeights();
        stream << polygon.reflectionWeights();
        stream << polygon.reflectionCoefficient() << polygon.cosPower() << verticesCount;

        //Запись координат вершин многоугольника.
        for (int vertInd = 0; vertInd < verticesCount; ++vertInd)
        {
            const Point &pnt = polygon.vertice(vertInd);
            stream << pnt.x;
            stream << pnt.y;
            stream << pnt.z;
        }
    }

    //Запись в файл источников света.
    for (const auto &light: lights)
    {
        const Point &place = light.Place();
        stream << place.x;
        stream << place.y;
        stream << place.z;
        stream << light.Intensivity();
    }

    // Update settings.
    if (settings.get() != nullptr)
        settings->setValue(PATH_SETTING_NAME, path);
}

void MainWindow::loadScene()
{
    std::unique_ptr<QSettings> settings = settingsInst();
    const QString initialPath = (settings.get() != nullptr) ?
        settings->value(PATH_SETTING_NAME).toString() : QString();

    const QString path = QFileDialog::getOpenFileName(this,
        tr("Загрузить из файла..."), initialPath,
        QString(), nullptr, QFileDialog::DontUseNativeDialog);
    if(path.isEmpty())
        return;

    QFile file(path);
    const bool isOpen = file.open(QIODevice::ReadOnly);
    if (!isOpen) {
        QMessageBox::warning(this, tr("Загрузка сцены"),
            tr("Невозможно открыть файл\n\"%1\"").arg(path));
        return;
    }
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setFloatingPointPrecision(QDataStream::SinglePrecision);

    float r[2], g[2], b[2];
    float reflCoeff = 0.f;
    int cosPow = 0;
    int vertCount = 0;

    _scene.clear();

    //Считывание кол-в многоугольников и источников света.
    int np = 0;
    int nl = 0;
    stream >> np;
    stream >> nl;

    //Чтение многоугольников.
    std::vector<Polygon> polygons;
    polygons.reserve(np);
    for(int i = 0; i < np; ++i) {
        //Считывание коэффициентов и кол-ва вершин очередного многоугольника.
        stream >> r[0] >> g[0] >> b[0];
        stream >> r[1] >> g[1] >> b[1];
        stream >> reflCoeff >> cosPow >> vertCount;

        //Считывание массива вершин очередного многоуогольника.
        std::vector<Point> vertices;
        vertices.reserve(vertCount);
        for(int j = 0; j < vertCount; ++j) {
            float x, y, z;
            stream >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }

        //"Добавление" (т.е. изменение) очередного многоугольника.
        polygons.emplace_back(vertices, Rgb(r[0], g[0], b[0]),
            Rgb(r[1], g[1], b[1]), reflCoeff, cosPow);
    }

    //Чтение источников света.
    std::vector<SpotLight> lights;
    lights.reserve(nl);
    for(int i = 0; i < nl; ++i)
    {
        float x, y, z, intensivity;
        stream >> x >> y >> z >> intensivity;
        lights.emplace_back(x, y, z, intensivity);
    }

    _scene.setPolygons(std::move(polygons));
    _scene.setLights(std::move(lights));

    // Update settings.
    if (settings.get() != nullptr)
        settings->setValue(PATH_SETTING_NAME, path);
}

void MainWindow::addPolygon()
{
    AddPolygonDialog dialog(this);
    const int res = dialog.exec();
    if (res == QDialog::Accepted) {
        _scene.addPolygon(dialog.polygon());
    }
}

void MainWindow::addLight()
{
    AddLightDialog dialog(this);
    const int res = dialog.exec();
    if (res == QDialog::Accepted) {
        _scene.addLight(dialog.light());
    }
}

void MainWindow::editScene()
{
    EditSceneDialog dialog(_scene, this);
    const int res = dialog.exec();
    if (res == QDialog::Accepted) {
        _scene = dialog.scene();
    }
}

void MainWindow::handleRenderStart()
{
    setEnabled(false);

    _progressBar->setVisible(false);
    _progressBar->setValue(0);
    statusBar()->showMessage(tr("Отрисовка начата"));
}

void MainWindow::handleRenderFinish()
{
    const float drawingTime = float(_timer.elapsed()) * 0.001f;
    statusBar()->showMessage(
        tr("Отрисовка завершена. Время: %1 секунд").
        arg(drawingTime));
    _progressBar->setVisible(false);

    setCanvas(_renderer->result());
    setEnabled(true);
    _renderingIsRunning = false;
}

void MainWindow::updateRenderProgress(float weight)
{
    if (_progressBar->isHidden())
        _progressBar->setVisible(true);
    _progressBar->setValue(weight * 100.f + 0.5f);
}

void MainWindow::render()
{
    if (_renderer.isNull()) {
        QMessageBox::critical(this, tr("Ошибка отрисовки"),
            tr("Алгоритм рендеринга не может запущен. "
               "Попробуйте перезапустить программу."));
        return;
    }

    _renderingIsRunning = true;
    _timer.start();

    const QWidget *display = _ui->pixmapWidget;
    _renderer->renderAsync(_scene, display->size());
}
