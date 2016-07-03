#ifndef EDITSCENEDIALOG_H
#define EDITSCENEDIALOG_H

#include <QDialog>

#include "Scene.h"

namespace Ui {
class EditSceneDialog;
}

class EditSceneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSceneDialog(const Scene &initialScene, QWidget *parent = 0);
    ~EditSceneDialog();

private:
    Ui::EditSceneDialog *_ui;
};

#endif // EDITSCENEDIALOG_H
