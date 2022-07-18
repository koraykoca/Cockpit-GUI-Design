#include "positionsource.h"
#include "Point.h"
#include "Location.h"

using namespace Esri::ArcGISRuntime;


PositionSource::PositionSource(ZmqReciever* zmqReciever,QObject* parent) :
    Esri::ArcGISRuntime::AbstractLocationDataSource(parent)
{
    m_zmqReciever = zmqReciever;

    QObject::connect( zmqReciever, &ZmqReciever::gpsPositionChanged,
                      this, &PositionSource::updateFromZmq
                      );

}

PositionSource::~PositionSource()
{}

void PositionSource::onStart(){
    // TO DO
}

void PositionSource::onStop(){
    // TO DO
}

void PositionSource::updateFromZmq(QVector<float> newLocation){

    float lat = newLocation[0];
    float lon = newLocation[1];
    Point point(lat, lon, SpatialReference::wgs84());
    Location location(point, 10,10,10,10); //random values
    updateLocation(location);
    qDebug() << "called this here form Position source";


}

