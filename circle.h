#ifndef CIRCLE_H
#define CIRCLE_H


#include <QWidget>
#include <QDialog>
#include <QtGui>

class Circle : public QDialog
{
    Q_OBJECT
public:
    explicit Circle(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent* e) override;
    void closeEvent(QCloseEvent* e) override;

signals:

};

#endif // CIRCLE_H
