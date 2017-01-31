#include "AddPolygonDialog.h"
#include "ui_AddPolygonDialog.h"

#include <set>

#include <QStandardItemModel>

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

    _coeffModel = createCoeffModel(this);
    _ui->colorCoeffTable->setModel(_coeffModel);
    _ui->colorCoeffTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    connect(_ui->addVerticeButton, &QPushButton::clicked,
            this, &AddPolygonDialog::addVertice);
    connect(_ui->removeVerticeButton, &QPushButton::clicked,
            this, &AddPolygonDialog::removeVertice);

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
    // TODO: check to at least 3 vertices defined.
    const std::vector<Point> vertices = readVertices();
    const std::pair<Rgb, Rgb> rgbs = readRgbs();
    return Polygon(vertices, rgbs.first, rgbs.second,
        _ui->ksSpin->value(), _ui->nSpin->value());
}

QStandardItemModel *AddPolygonDialog::createCoeffModel(QObject *parent)
{
    QStandardItemModel *model = new QStandardItemModel(parent);
    const int rowCount = 3;
    const int colCount = 2;
    model->setRowCount(rowCount);
    model->setColumnCount(colCount);

    model->setHorizontalHeaderLabels(QStringList() << "ka" << "kd");

    const auto setColoredText =
        [model](int row, const QString &text, const QColor &color) {
        model->setHeaderData(row, Qt::Vertical, text, Qt::DisplayRole);
        model->setHeaderData(row, Qt::Vertical, color, Qt::ForegroundRole);
    };
    setColoredText(0, "R", Qt::red);
    setColoredText(1, "G", Qt::green);
    setColoredText(2, "B", Qt::blue);

    for (int row = 0; row < rowCount; ++row)
        for (int col = 0; col < colCount; ++col)
            model->setData(model->index(row, col), 0., Qt::DisplayRole);

    return model;
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
    const auto readColor = [&](int row, int col) -> float {
        return _coeffModel->data(_coeffModel->index(row, col),
            Qt::DisplayRole).toFloat();
    };
    const Rgb ka(readColor(0, 0), readColor(1, 0), readColor(2, 0));
    const Rgb kd(readColor(0, 1), readColor(1, 1), readColor(2, 1));

    return std::pair<Rgb, Rgb>(ka, kd);
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
}

void AddPolygonDialog::removeVertice()
{
    const QModelIndexList toDelete =
        _ui->verticesTable->selectionModel()->selectedIndexes();

    std::set<int> rowsToDelete;
    for (const auto &index: toDelete)
        rowsToDelete.insert(index.row());

    const auto rem = [this](int row) {
        _ui->verticesTable->removeRow(row);
    };
    std::for_each(rowsToDelete.crbegin(), rowsToDelete.crend(), rem);
}
