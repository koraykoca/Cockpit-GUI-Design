#ifndef POSITIONSOURCE_H
#define POSITIONSOURCE_H

#include "AbstractLocationDataSource.h"
#include "Location.h"
#include "zmqreciever.h"


namespace Esri
{
namespace ArcGISRuntime
{

class Point;
class Location;

}
}
class PositionSource : public Esri::ArcGISRuntime::AbstractLocationDataSource
{
Q_OBJECT

public:
explicit PositionSource(ZmqReciever* zmqReciever, QObject* parent = nullptr);
~PositionSource();

 void onStart() override;
 void onStop() override;
public slots:
 void updateFromZmq(QVector<float> newLocation);


private:

 ZmqReciever* m_zmqReciever = nullptr;

};
#endif // POSITIONSOURCE_H
