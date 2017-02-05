#include "MainWindow.h"
#include <QApplication>

//#include "AddPolygonDialog.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("ilotXXI-open-source");
    QApplication app(argc, argv);

    MainWindow window;
//    AddPolygonDialog window;
    window.show();

    return app.exec();
}
