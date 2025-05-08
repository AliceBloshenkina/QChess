#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");

    a.setWindowIcon(QIcon("C:/Users/User/Documents/build-untitled1-Desktop_Qt_6_6_1_MinGW_64_bit-Release/icon.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
