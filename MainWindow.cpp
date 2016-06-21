#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTime>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "Polygon.h"
#include "SpotLight.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    connect(_ui->N2, &QAction::triggered, this, &MainWindow::saveScene);
    connect(_ui->N3, &QAction::triggered, this, &MainWindow::loadScene);
    connect(_ui->N4, &QAction::triggered, this, &MainWindow::close);
    connect(_ui->N5, &QAction::triggered, this, &MainWindow::newFile);
    connect(_ui->N7, &QAction::triggered, this, &MainWindow::render);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setCanvas(const Canvas &canvas)
{
    if (canvas.empty() || canvas.front().empty()) {
        _ui->pixmapWidget->setPixmap(QPixmap());
        return;
    }

    const int width = canvas.size();
    const int height = canvas.front().size();
    QImage image(width, height, QImage::Format_RGB32);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            image.setPixel(x, y, canvas[x][y].rgb());
        }
    }

    _ui->pixmapWidget->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::newFile()
{
    _obj.clear();
    _lights.clear();
}

void MainWindow::saveScene()
{
    const QString path =
        QFileDialog::getSaveFileName(this, "Записать в файл...");
    if(path.isEmpty())
        return;

    //Открытие файла.
    QFile file(path);
    const bool isOpen = file.open(QIODevice::WriteOnly);
    if (!isOpen) {
        QMessageBox::warning(this, tr("Запись сцены"),
            tr("Невозможно открыть файл\n\"%1\"").arg(path));
        return;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    //Запись в файл кол-в многоугольников и источников света.
    stream << _obj.size() << _lights.size();

    //Запись в файл многоугольников.
    for (const auto &polygon:    _obj)
    {
        const int verticesCount = polygon.VerticesCount();

        //Запись коэффициентов и кол-ва вершин очередного многоугольника.
        stream << polygon.Rka << polygon.Gka << polygon.Bka;
        stream << polygon.Rkd << polygon.Gkd << polygon.Bkd;
        stream << polygon.ks << polygon.c_p_k << verticesCount;

        //Запись координат вершин многоугольника.
        for (int vertInd = 0; vertInd < verticesCount; ++vertInd)
        {
            const point &pnt = polygon.Vertice(vertInd);
            stream << pnt.x;
            stream << pnt.y;
            stream << pnt.z;
        }
    }

    //Запись в файл источников света.
    for (const auto &light: _lights)
    {
        const point &place = light.Place();
        stream << place.x;
        stream << place.y;
        stream << place.z;
        stream << light.Intensivity();
    }
}

void MainWindow::loadScene()
{
    const QString path =
        QFileDialog::getOpenFileName(this, tr("Загрузить из файла..."));
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
    float x[25], y[25], z[25];
    float ks = 0.f;
    int c_p_k = 0;
    int top_n = 0;

    _obj.clear();

    //Считывание кол-в многоугольников и источников света.
    int np = 0;
    int nl = 0;
    stream >> np;
    stream >> nl;

    //Чтение многоугольников.
    _obj.clear();
    for(int i = 0; i < np; ++i) {
        //Считывание коэффициентов и кол-ва вершин очередного многоугольника.
        stream >> r[0] >> g[0] >> b[0];
        stream >> r[1] >> g[1] >> b[1];
        stream >> ks >> c_p_k >> top_n;

        //Считывание массива вершин очередного многоуогольника.
        for(int j = 0; j < top_n; ++j)
            stream >> x[j] >> y[j] >> z[j];

        //"Добавление" (т.е. изменение) очередного многоугольника.
        _obj.emplace_back(x, y, z, top_n, r, g, b, ks, c_p_k);
    }

    //Чтение источников света.
    _lights.clear();
    for(int i = 0; i < nl; ++i)
    {
        stream >> *x >> *y >> *z >> *r;
        _lights.emplace_back(x[0], y[0], z[0], r[0]);
    }
}

void MainWindow::render()
{
    QTime timer;
    timer.start();

    const QWidget *display = _ui->pixmapWidget;
    Canvas canvas(display->width(),
        std::vector<QColor>(display->height(), Qt::black));
    Draw(canvas, _obj.data(), _obj.size(), _lights.data(), _lights.size());

    const float drawingTime = float(timer.elapsed()) * 0.001;
    statusBar()->showMessage(tr("%1 секунд").arg(drawingTime));

    setCanvas(canvas);
}
