#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QFontDatabase::addApplicationFont(":/resources/EASports.ttf");


    return a.exec();
}
