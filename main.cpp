#include "MainWindow.h"
#include <QApplication>

//#include "AddLightDialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
//    AddLightDialog window;
    window.show();

    return app.exec();
}
