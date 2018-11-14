#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "resourcecontainer.h"

int main(int argc, char *argv[])
{
    ResourceContainer rc;
    rc.m_cpuUsage[10] = 4.21312;
    for (int i = 0; i < 50; i++) {
        qDebug() << i << " : " << rc.m_cpuUsage[i];
    }
    return 0;
}
