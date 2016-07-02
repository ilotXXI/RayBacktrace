#include "AddLightDialog.h"
#include "ui_AddLightDialog.h"

#include "SpotLight.h"

AddLightDialog::AddLightDialog(QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::AddLightDialog)
{
    _ui->setupUi(this);

    connect(_ui->buttonBox, &QDialogButtonBox::accepted,
        this, &AddLightDialog::accept);
    connect(_ui->buttonBox, &QDialogButtonBox::rejected,
        this, &AddLightDialog::reject);
}

AddLightDialog::~AddLightDialog()
{
    delete _ui;
}

SpotLight AddLightDialog::light() const
{
    const float x = _ui->xSpin->value();
    const float y = _ui->ySpin->value();
    const float z = _ui->zSpin->value();
    const float intensiv = _ui->intensivitySpin->value();
    return SpotLight(x, y, z, intensiv);
}
