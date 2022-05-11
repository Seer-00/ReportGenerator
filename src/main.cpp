#include <QtWidgets/QApplication>
#include "qt_mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();
}
