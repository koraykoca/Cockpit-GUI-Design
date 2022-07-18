#ifndef POSITIONSOURCESIMULATOR_H
#define POSITIONSOURCESIMULATOR_H
#include <QGeoPositionInfoSource>
#include "zmqreciever.h"

class PositionSourceSimulator:public QGeoPositionInfoSource
{
public:
    explicit PositionSourceSimulator( ZmqReciever* zmqReciever, QObject* parent = nullptr);
    ~PositionSourceSimulator();
    QGeoPositionInfo lastKnownPosition(bool fromSatellitePositioningMethodsOnly = false) const override;
    PositioningMethods supportedPositioningMethods() const override;
    int minimumUpdateInterval() const override;
    QGeoPositionInfoSource::Error error() const override;

public slots:
    void startUpdates() override;
    void stopUpdates() override;
    void requestUpdate(int timeout = 0) override;
    void updateFromZmq(QVector<  double> newLocation);

private:

    QGeoPositionInfo m_currentPosition;
    QGeoPositionInfoSource::Error m_lastError = QGeoPositionInfoSource::NoError;

    bool m_started;
    ZmqReciever* m_zmqReciever = nullptr;

};

#endif // POSITIONSOURCESIMULATOR_H
