#include "EditSceneDialog.h"
#include "ui_EditSceneDialog.h"

#include "PolygonsTable.h"
#include "LightsTable.h"
#include "Delegate.h"

EditSceneDialog::EditSceneDialog(const Scene &initialScene, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditSceneDialog)
{
    _ui->setupUi(this);

    _polTable = new PolygonsTable(initialScene.polygons(), this);
    _ui->polygonsView->setModel(_polTable);
    _ui->polygonsView->setItemDelegate(new Delegate(this));

    _lightsTable = new LightsTable(initialScene.lights(), this);
    _ui->lightsView->setModel(_lightsTable);
    Delegate *lightsDelegate = new Delegate(_ui->lightsView);
    lightsDelegate->setDoubleRange(- 1E6, 1E6);
    lightsDelegate->setColumnDoubleRange(LightsTable::Intensivity, 0., 255.);
    lightsDelegate->setDoubleStep(10.);
    _ui->lightsView->setItemDelegate(lightsDelegate);

    connect(_ui->buttonBox, &QDialogButtonBox::accepted,
        this, &QDialog::accept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

EditSceneDialog::~EditSceneDialog()
{
    delete _ui;
    _ui = nullptr;
    delete _polTable;
    _polTable = nullptr;
    delete _lightsTable;
    _lightsTable = nullptr;
}

Scene EditSceneDialog::scene() const
{
    return Scene(_polTable->polygons(), _lightsTable->lights());
}
