#include "overlay.h"


Overlay::Overlay(QWidget *parent)
    : QWidget{parent}
{
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Overlay::paintEvent(QPaintEvent* e)
{       Q_UNUSED(e);
        QPainter(this).fillRect(rect(), {80, 80, 255, 128});
}
