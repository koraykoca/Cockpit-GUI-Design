#include "circle.h"

Circle::Circle(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Circle::paintEvent(QPaintEvent* e)
{
    int x = 40;
    int y = 30;
    int radius = 15;
    QPainter circlePainter(this);
    circlePainter.setRenderHint(QPainter::Antialiasing, true);
    QPen circlePen;
    circlePen.setColor(Qt::green);
    circlePen.setWidth(3);
    circlePainter.setPen(circlePen);
    circlePainter.drawEllipse(QPointF(x,y), radius, radius);
    Q_UNUSED(e);

}
