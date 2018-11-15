#ifndef TIMEPLOT_H
#define TIMEPLOT_H

#include <QWidget>
#include <QPaintEvent>
#include <QRect>
#include <QTimer>
#include <QLabel>

#include "resourcecontainer.h"

#define SEPARATION 5
#define HEIGHT 200
#define WIDTH 490

class Timeplot : public QWidget
{
    Q_OBJECT

private:
    const QRect CPU_RECT {20, 50, WIDTH, HEIGHT};
    const QRect MEMORY_RECT {20, 100 + HEIGHT, WIDTH, HEIGHT};
    ResourceContainer m_resources;
    QTimer *timer;

    void paintPlot(QPainter &painter);
    void paintContainer(QPainter &painter);

public:
    QLabel *m_cpuLabel;
    QLabel *m_memoryLabel;

    explicit Timeplot(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);

public slots:
    void update();
};

#endif // TIMEPLOT_H
