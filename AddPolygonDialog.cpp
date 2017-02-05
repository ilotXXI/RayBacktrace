#include "AddPolygonDialog.h"
#include "ui_AddPolygonDialog.h"

#include <set>

#include <QMessageBox>
#include <QColor>
#include <QColorDialog>

#include "Rgb.h"
#include "Point.h"

AddPolygonDialog::AddPolygonDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AddPolygonDialog)
{
    _ui->setupUi(this);

    QTableWidget *vertTable = _ui->verticesTable;
    vertTable->setHorizontalHeaderLabels(QStringList() << "x" << "y" << "z");
    vertTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setupDefaultData();

    connect(_ui->addVerticeButton, &QPushButton::clicked,
        this, &AddPolygonDialog::addVertice);
    connect(_ui->removeVerticeButton, &QPushButton::clicked,
        this, &AddPolygonDialog::removeVertice);
    connect(_ui->verticesTable->selectionModel(),
        &QItemSelectionModel::selectionChanged, this,
        &AddPolygonDialog::updateRemoveEnabled);

    connect(_ui->diffusedButton, &QPushButton::clicked,
        [this] () { askAndSetColor(_ui->diffusedButton); });
    connect(_ui->reflButton, &QPushButton::clicked,
        [this] () { askAndSetColor(_ui->reflButton); });

    connect(_ui->buttonBox, &QDialogButtonBox::accepted,
        this, &AddPolygonDialog::accept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected,
        this, &AddPolygonDialog::reject);
}

AddPolygonDialog::~AddPolygonDialog()
{
    delete _ui;
}

Polygon AddPolygonDialog::polygon() const
{
    const std::vector<Point> vertices = readVertices();
    Q_ASSERT_X(vertices.size() >= 3, "AddPolygonDialog::polygon() const",
        "Trying to get an invalid polygon.");

    const std::pair<Rgb, Rgb> rgbs = readRgbs();
    return Polygon(vertices, rgbs.first, rgbs.second,
                   _ui->reflCoeffSpin->value(), _ui->cosPowSpin->value());
}

void AddPolygonDialog::accept()
{
    if (_ui->verticesTable->rowCount() < 3) {
        QMessageBox::warning(this, tr("Добавление многоугольника"),
            tr("Задан недопустимый многоугольник. \n"
               "Он должен быть хотя бы треугольником."));
        return;
    }
    QDialog::accept();
}

void AddPolygonDialog::setupDefaultData()
{
    QAbstractItemModel *verModel= _ui->verticesTable->model();
    const Point defPoints[] = {{- 100, 0, 0}, {100, 0, 0}, {0, 100, 0}};
    int defPointsCount = int(sizeof(defPoints) / sizeof(*defPoints));

    verModel->insertRows(0, defPointsCount);
    for (int i = 0; i < defPointsCount; ++i) {
        const Point &point = defPoints[i];
        verModel->setData(verModel->index(i, 0), double(point.x));
        verModel->setData(verModel->index(i, 1), double(point.y));
        verModel->setData(verModel->index(i, 2), double(point.z));
    }

    updateRemoveEnabled();

    setColor(_ui->diffusedButton, Qt::white);
    setColor(_ui->reflButton, Qt::white);
}

std::vector<Point> AddPolygonDialog::readVertices() const
{
    std::vector<Point> vertices;
    const int verticesCount = _ui->verticesTable->rowCount();
    vertices.reserve(verticesCount);
    const QAbstractItemModel *model = _ui->verticesTable->model();
    const auto extractVal = [model](int row, int col) -> float {
        return model->data(model->index(row, col), Qt::DisplayRole).toFloat();
    };

    for (int i = 0; i < verticesCount; ++i) {
        const float x = extractVal(i, 0);
        const float y = extractVal(i, 1);
        const float z = extractVal(i, 2);
        vertices.emplace_back(x, y, z);
    }

    return vertices;
}

std::pair<Rgb, Rgb> AddPolygonDialog::readRgbs() const
{
    const auto readColor = [] (const QPushButton *button) -> Rgb {
        const QColor color = button->palette().color(QPalette::Button);
        return Rgb(float(color.redF()),
            float(color.greenF()), float(color.blueF()));
    };
    const Rgb ka = readColor(_ui->diffusedButton);
    const Rgb kd = readColor(_ui->reflButton);

    return std::pair<Rgb, Rgb>(ka, kd);
}

inline bool AddPolygonDialog::canRemove() const
{
    const QTableWidget *table = _ui->verticesTable;
    const QModelIndexList sel = table->selectionModel()->selectedRows();
    return !sel.empty() && (table->rowCount() - sel.size() >= 3);
}

void AddPolygonDialog::setColor(QPushButton *toButton, const QColor &color)
{
    QPalette pal = toButton->palette();
    const QColor textColor(0xFFFFFFFF - color.rgb());
    pal.setColor(QPalette::Button, color);
    pal.setColor(QPalette::ButtonText, textColor);
    toButton->setPalette(pal);
}

inline QColor AddPolygonDialog::color(QPushButton *ofButton)
{
    return ofButton->palette().color(QPalette::Button);
}

void AddPolygonDialog::askAndSetColor(QPushButton *forButton)
{
    const QColor selectedCol =
        QColorDialog::getColor(color(forButton), this, tr("Выберите цвет"));
    if (!selectedCol.isValid())
        return;

    setColor(forButton, selectedCol);
}

void AddPolygonDialog::updateRemoveEnabled()
{
    _ui->removeVerticeButton->setEnabled(canRemove());
}

void AddPolygonDialog::addVertice()
{
    QTableWidget *table = _ui->verticesTable;

    const int newRow = table->rowCount();
    table->setRowCount(newRow + 1);

    const int colCount = table->columnCount();
    QAbstractItemModel *model = table->model();
    for (int col = 0; col < colCount; ++col)
        model->setData(model->index(newRow, col), 0., Qt::DisplayRole);

    updateRemoveEnabled();
}

void AddPolygonDialog::removeVertice()
{
    if (!canRemove()) {
        QMessageBox::warning(this, tr("Удаление вершины"),
            tr("Многоугольник с менее, чем 3 сторонами, недопустим."));
        return;
    }

    QTableWidget *verTable = _ui->verticesTable;
    const QModelIndexList toDelete =
        verTable->selectionModel()->selectedIndexes();

    std::set<int> rowsToDelete;
    for (const auto &index: toDelete)
        rowsToDelete.insert(index.row());

    const auto rem = [verTable] (int row) {
        verTable->removeRow(row);
    };
    std::for_each(rowsToDelete.crbegin(), rowsToDelete.crend(), rem);

    updateRemoveEnabled();
}
