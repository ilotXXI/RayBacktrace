#include "EditSceneDialog.h"
#include "ui_EditSceneDialog.h"

#include "PolygonsTable.h"
#include "Delegate.h"

EditSceneDialog::EditSceneDialog(const Scene &initialScene, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::EditSceneDialog)
    , _lights(initialScene.lights())
{
    _ui->setupUi(this);

    _polTable = new PolygonsTable(initialScene.polygons(), this);
    _ui->polygonsView->setModel(_polTable);
    _ui->polygonsView->setItemDelegate(new Delegate(this));

    connect(_ui->buttonBox, &QDialogButtonBox::accepted,
        this, &QDialog::accept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected,
        this, &QDialog::reject);
}

EditSceneDialog::~EditSceneDialog()
{
    delete _ui;
}

Scene EditSceneDialog::scene() const
{
    return Scene(_polTable->polygons(), _lights);
}
