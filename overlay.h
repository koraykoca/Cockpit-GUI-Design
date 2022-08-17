#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QtGui>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;

signals:

};

#endif // OVERLAY_H
