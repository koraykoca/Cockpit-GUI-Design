#include"positionsourcesimulator.h"
#include "Point.h"
#include "Location.h"
using namespace Esri::ArcGISRuntime;

PositionSourceSimulator::PositionSourceSimulator(ZmqReciever* zmqReciever, QObject* parent) :
    QGeoPositionInfoSource(parent)
{
    setUpdateInterval(500);
    m_currentPosition.setCoordinate(QGeoCoordinate(52.920905, 4.862173));
    emit positionUpdated(m_currentPosition);
    m_zmqReciever = zmqReciever;
    QObject::connect( zmqReciever, &ZmqReciever::gpsPositionChanged,
                      this, &PositionSourceSimulator::updateFromZmq
                      );
}

PositionSourceSimulator::~PositionSourceSimulator()
{}

void PositionSourceSimulator::startUpdates()
{
    if(m_started)
        return;

    m_started = true;
    //TODO: start communication with bridge
}

void PositionSourceSimulator::stopUpdates()
{
    if(!m_started)
        return;

    m_started = false;

    //TODO: stop communication with bridge
}

void PositionSourceSimulator::requestUpdate(int timeout)
{
    Q_UNIMPLEMENTED();
}

QGeoPositionInfo PositionSourceSimulator::lastKnownPosition(bool fromSatellitePositioningMethodsOnly) const
{
    Q_UNUSED(fromSatellitePositioningMethodsOnly);
    return m_currentPosition;
}

QGeoPositionInfoSource::PositioningMethods PositionSourceSimulator::supportedPositioningMethods() const
{
    return QGeoPositionInfoSource::PositioningMethod::NoPositioningMethods;
}

int PositionSourceSimulator::minimumUpdateInterval() const
{
    return updateInterval();
}

QGeoPositionInfoSource::Error PositionSourceSimulator::error() const
{
    return m_lastError;
}


void PositionSourceSimulator::updateFromZmq(QVector<  double> newLocation){

    double lat = newLocation[0];
    double lon = newLocation[1];
    // double heading = newLocation[5];
    Point point(lat, lon, SpatialReference::wgs84());
    Location location(point, 10,10,10,10); //random values
    m_currentPosition.setCoordinate(QGeoCoordinate(lat,lon));
    // m_currentPosition.setAttribute(QGeoPositionInfo::Direction, heading);
    emit positionUpdated(m_currentPosition);
    //qDebug() << "called this here form Position source";


}

