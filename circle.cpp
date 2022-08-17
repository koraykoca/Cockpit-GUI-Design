#include "circle.h"

Circle::Circle(QWidget *parent)
    : QDialog{parent}
{

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TransparentForMouseEvents);
//    setWindowFlag(Qt::FramelessWindowHint);
    QTimer::singleShot(5000, this, SLOT(close()));
    adjustSize();


}

void Circle::paintEvent(QPaintEvent* e)
{
    int x = 40;
    int y = 30;
    int radius = 20;
    QPainter circlePainter(this);
    circlePainter.setRenderHint(QPainter::Antialiasing, true);
    QPen circlePen;
    circlePen.setColor(Qt::green);
    circlePen.setWidth(3);
    circlePainter.setPen(circlePen);
    circlePainter.drawEllipse(QPointF(x,y), radius, radius);


    Q_UNUSED(e);

}

void Circle::closeEvent(QCloseEvent *e)
{   qDebug() << "closing circular marker" ;
    QWidget::setWindowState(Qt::WindowMinimized);
    QWidget::close();

    Q_UNUSED(e);
}
