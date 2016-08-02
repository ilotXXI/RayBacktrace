#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTime>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include <memory>

#include "AddPolygonDialog.h"
#include "AddLightDialog.h"
#include "Polygon.h"
#include "SpotLight.h"
#include "EditSceneDialog.h"
#include "Canvas.h"

static const char *pathSettingName = "filePath";
static const char *geometrySettingName = "geometry";

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
{
    _ui->setupUi(this);

    const std::unique_ptr<QSettings> settings = settingsInst();
    if (settings.get() != nullptr)
        restoreGeometry(settings->value(geometrySettingName).toByteArray());

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
}

MainWindow::~MainWindow()
{
    const std::unique_ptr<QSettings> settings = settingsInst();
    if (settings.get() != nullptr)
        settings->setValue(geometrySettingName, saveGeometry());
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

void MainWindow::newFile()
{
    _scene.clear();
}

void MainWindow::saveScene() const
{
    std::unique_ptr<QSettings> settings = settingsInst();
    const QString initialPath = (settings.get() != nullptr) ?
        settings->value(pathSettingName).toString() : QString();

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
        const int verticesCount = polygon.VerticesCount();

        //Запись коэффициентов и кол-ва вершин очередного многоугольника.
        stream << polygon.KaColor();
        stream << polygon.KdColor();
        stream << polygon.GetKs() << polygon.CosPower() << verticesCount;

        //Запись координат вершин многоугольника.
        for (int vertInd = 0; vertInd < verticesCount; ++vertInd)
        {
            const Point &pnt = polygon.Vertice(vertInd);
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
        settings->setValue(pathSettingName, path);
}

void MainWindow::loadScene()
{
    std::unique_ptr<QSettings> settings = settingsInst();
    const QString initialPath = (settings.get() != nullptr) ?
        settings->value(pathSettingName).toString() : QString();

    const QString path = QFileDialog::getOpenFileName(this,
        tr("Загрузить из файла..."), initialPath);
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
    float ks = 0.f;
    int c_p_k = 0;
    int top_n = 0;

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
        stream >> ks >> c_p_k >> top_n;

        //Считывание массива вершин очередного многоуогольника.
        std::vector<Point> vertices;
        vertices.reserve(top_n);
        for(int j = 0; j < top_n; ++j) {
            float x, y, z;
            stream >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }

        //"Добавление" (т.е. изменение) очередного многоугольника.
        polygons.emplace_back(vertices, Rgb(r[0], g[0], b[0]),
            Rgb(r[1], g[1], b[1]), ks, c_p_k);
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
        settings->setValue(pathSettingName, path);
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

void MainWindow::render()
{
    QTime timer;
    timer.start();

    const QWidget *display = _ui->pixmapWidget;
    Canvas canvas(display->width(), display->height());
    const std::vector<Polygon> &pol = _scene.polygons();
    const std::vector<SpotLight> &lights = _scene.lights();
    Draw(canvas, pol.data(), pol.size(), lights.data(), lights.size());

    const float drawingTime = float(timer.elapsed()) * 0.001;
    statusBar()->showMessage(tr("%1 секунд").arg(drawingTime));

    setCanvas(canvas);
}
