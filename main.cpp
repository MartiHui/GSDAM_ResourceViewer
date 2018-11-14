#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <chrono>
#include <ctime>

#include "resourcecontainer.h"

int main(int argc, char *argv[])
{
    ResourceContainer rc;
    rc.getInfo();
    return 0;
}
