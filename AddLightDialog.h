#ifndef ADDLIGHTDIALOG_H
#define ADDLIGHTDIALOG_H

#include <QDialog>

namespace Ui {
class AddLightDialog;
}
class SpotLight;

class AddLightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddLightDialog(QWidget *parent = nullptr);
    ~AddLightDialog();

    SpotLight light() const;

private:
    Ui::AddLightDialog  *_ui;
};

#endif // ADDLIGHTDIALOG_H
