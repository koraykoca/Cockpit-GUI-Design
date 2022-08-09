#ifndef CIRCLE_H
#define CIRCLE_H


#include <QWidget>
#include <QtGui>

class Circle : public QWidget
{
    Q_OBJECT
public:
    explicit Circle(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;

signals:

};

#endif // CIRCLE_H
