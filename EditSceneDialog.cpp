#include "EditSceneDialog.h"
#include "ui_EditSceneDialog.h"

#include "PolygonsTable.h"

EditSceneDialog::EditSceneDialog(const Scene &initialScene, QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::EditSceneDialog)
{
    _ui->setupUi(this);

    PolygonsTable *polTable = new PolygonsTable(initialScene.polygons(), this);
    _ui->polygonsView->setModel(polTable);
}

EditSceneDialog::~EditSceneDialog()
{
    delete _ui;
}
