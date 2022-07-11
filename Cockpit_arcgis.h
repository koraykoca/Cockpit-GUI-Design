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

#ifndef COCKPIT_ARCGIS_H
#define COCKPIT_ARCGIS_H

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
class Point;
class SimulatedLocationDataSource;
}
}

#include <QMainWindow>
#include "ui_Cockpit_arcgis.h"
#include <QVBoxLayout>

#include "positionsourcesimulator.h"



class cockpitArcgis : public QMainWindow
{
    Q_OBJECT
public:
    explicit cockpitArcgis(QWidget* parent = nullptr);
    ~cockpitArcgis() override;

public slots:
    void getCoordinate(QMouseEvent&);
    void displayCoordinate(QMouseEvent&);
    void planeGpsPositionChanged(QVector<  double> newPosition);

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
    Ui::MainWindow*                             ui;
    std::unique_ptr<QVBoxLayout>                layoutMap;
    PositionSourceSimulator* m_positionSourceSimulator = nullptr;

    std::vector<QUrl> m_urlVectors;


    void setupViewPoint();
    void addLayer(QUrl);
    void addMarker();
    void updateMarker(Esri::ArcGISRuntime::Point);
    void setLayersUrlVector();
    void readGpsFromXplane();
};

#endif // COCKPIT_ARCGIS_H
