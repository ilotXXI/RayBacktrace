#ifndef EDITSCENEDIALOG_H
#define EDITSCENEDIALOG_H

#include <QDialog>

#include "Scene.h"

namespace Ui {
class EditSceneDialog;
}
class PolygonsTable;


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
    std::vector<SpotLight>  _lights;
};

#endif // EDITSCENEDIALOG_H
