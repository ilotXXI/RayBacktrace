#ifndef EDITSCENEDIALOG_H
#define EDITSCENEDIALOG_H

#include <QDialog>

#include "Scene.h"

namespace Ui {
class EditSceneDialog;
}
class PolygonsTable;
class LightsTable;


class EditSceneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSceneDialog(const Scene &initialScene, QWidget *parent = 0);
    ~EditSceneDialog();

    Scene scene() const;

private:
    Ui::EditSceneDialog     *_ui;

    PolygonsTable           *_polTable;
    LightsTable             *_lightsTable;
};

#endif // EDITSCENEDIALOG_H
