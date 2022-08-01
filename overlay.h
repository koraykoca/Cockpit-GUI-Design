#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QtGui>

class overlay : public QWidget
{
    Q_OBJECT
public:
    explicit overlay(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;

signals:

};

#endif // OVERLAY_H
