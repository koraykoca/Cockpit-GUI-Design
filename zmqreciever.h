#ifndef ZMQRECIEVER_H
#define ZMQRECIEVER_H

#include <QObject>

class ZmqReciever : public QObject
{
    Q_OBJECT

public:
    explicit ZmqReciever(QObject *parent = nullptr);

signals:
    void gpsPositionChanged(QVector<  double> gpsPositions );
private:
    void start();
    void execute();
};

#endif // ZMQRECIEVER_H
