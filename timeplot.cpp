#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPoint>
#include <QTimer>
#include <QDebug>

#include "timeplot.h"

Timeplot::Timeplot(QWidget *parent) : QWidget(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

void Timeplot::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    paintPlot(painter);
    paintContainer(painter);
}

void Timeplot::paintContainer(QPainter &painter) {
    QPen penRect(QColor(0, 0, 0, 255));
    penRect.setWidth(2);
    painter.setPen(penRect);
    painter.drawRect(CPU_RECT);
    painter.drawRect(MEMORY_RECT);
}

void Timeplot::paintPlot(QPainter &painter) {
    QPen penCpu(QColor(0, 0, 255, 255));
    penCpu.setWidth(2);
    QPen penMemory(QColor(255, 0, 0, 255));
    penMemory.setWidth(2);

    painter.fillRect(CPU_RECT, QColor(255, 255, 255, 255));
    painter.fillRect(MEMORY_RECT, QColor(255, 255, 255, 255));

    // Se multiplica la coordenada vertical por 2 porque el rectangulo mide 200
    // de alto, mientras que el porcentaje esta entre 0 y 100
    QPoint cpu_point1(CPU_RECT.left(), CPU_RECT.bottom() - m_resources.m_cpuUsage[0] * 2);
    QPoint cpu_point2;

    QPoint memory_point1(MEMORY_RECT.left(), MEMORY_RECT.bottom() - m_resources.m_memoryUsage[0] * 2);
    QPoint memory_point2;

    for (int i = 1; i < CONTAINER_SIZE; i++) {
        cpu_point2.setX(CPU_RECT.left() + 2*i*SEPARATION);
        cpu_point2.setY(CPU_RECT.bottom() - m_resources.m_cpuUsage[i]*2);
        painter.setPen(penCpu);
        painter.drawLine(cpu_point1, cpu_point2);
        cpu_point1 = cpu_point2;

        memory_point2.setX(MEMORY_RECT.left() + 2*i*SEPARATION);
        memory_point2.setY(MEMORY_RECT.bottom() - m_resources.m_memoryUsage[i]*2);
        painter.setPen(penMemory);
        painter.drawLine(memory_point1, memory_point2);
        memory_point1 = memory_point2;
    }

    m_cpuLabel->setText(QString::fromStdString("CPU: ") + QString::number(m_resources.m_cpuUsage[CONTAINER_SIZE - 1],'f', 2) + QString::fromStdString("%"));
    m_memoryLabel->setText(QString::fromStdString("RAM: ") + QString::number(m_resources.m_memoryUsage[CONTAINER_SIZE - 1],'f', 2) + QString::fromStdString("%"));
}

void Timeplot::update() {
    m_resources.getInfo();
    repaint();
}
