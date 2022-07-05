// Copyright 2022 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#ifndef COCKPITMAP_H
#define COCKPITMAP_H

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapQuickView;
class Point;
}
}
class QMouseEvent;
#include <QObject>

class CockpitMap : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::MapQuickView* mapView READ mapView WRITE setMapView NOTIFY mapViewChanged)

public:
    explicit CockpitMap(QObject* parent = nullptr);
    ~CockpitMap() override;

public slots:
    void getCoordinates(QMouseEvent& event);
signals:
    void mapViewChanged();


private:
    Esri::ArcGISRuntime::MapQuickView* mapView() const;

    void setMapView(Esri::ArcGISRuntime::MapQuickView* mapView);
    void setupViewPoint();
    void addLayer(QUrl);
    void addMarker();
    void updateMarker(Esri::ArcGISRuntime::Point);
    void setLayersUrlVector();

    Esri::ArcGISRuntime::Map* m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView* m_mapView = nullptr;


    std::vector<QUrl> m_urlVectors;
};

#endif // COCKPITMAP_H
