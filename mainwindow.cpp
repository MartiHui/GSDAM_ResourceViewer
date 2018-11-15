#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->m_cpuLabel = ui->widget->findChild<QLabel *>("cpuPercent");
    ui->widget->m_memoryLabel = ui->widget->findChild<QLabel *>("ramPercent");
}

MainWindow::~MainWindow()
{
    delete ui;
}
