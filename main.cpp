#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <chrono>
#include <ctime>

#include "resourcecontainer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
