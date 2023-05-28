#include "mainwindow.h"

#include <QApplication>

//Main funciton, nothing special

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
