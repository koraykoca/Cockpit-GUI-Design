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
class DefaultLocationDataSource;
}
}

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenu>
#include <QCheckBox>
#include <QSignalMapper>
#include <set>

#include "ui_Cockpit_arcgis.h"
#include "FeatureLayer.h"
#include "ServiceFeatureTable.h"
#include "Point.h"
#include "positionsourcesimulator.h"
#include "PictureMarkerSymbol.h"
#include "PolylineBuilder.h"

class cockpitArcgis : public QMainWindow
{
    Q_OBJECT
public:
    explicit cockpitArcgis(QWidget* parent = nullptr);
    ~cockpitArcgis() override;

public slots:
    void getCoordinate(QMouseEvent&);
    void displayCoordinate(QMouseEvent&);
    void addLayer(QUrl);
    void arrangeLayers(QString);
    void getCBoxState(int);
    void planeGpsPositionChanged(QVector<  double> newPosition);
    void updatesFromZmq(QVector<double> newAttributes);

private:
    Esri::ArcGISRuntime::Map*                                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*                       m_mapView = nullptr;
    Esri::ArcGISRuntime::DefaultLocationDataSource*             m_d = nullptr;
    Ui::MainWindow*                                             ui;
    std::unique_ptr<QVBoxLayout>                                layoutMap;
    QMenu*                                                      layerMenu;
    QList<QAction*>                                             actionList;
    QSignalMapper*                                              signalMapper;
    Esri::ArcGISRuntime::ServiceFeatureTable*                   ftrTable;
    Esri::ArcGISRuntime::FeatureLayer*                          ftrLayer;
    PositionSourceSimulator*                                    m_positionSourceSimulator = nullptr;
    std::map<Esri::ArcGISRuntime::FeatureLayer*, QUrl>          cBoxMap;
    std::unique_ptr<Esri::ArcGISRuntime::PictureMarkerSymbol>   planeMarker;
    QImage*                                                     planeIcon;
    Esri::ArcGISRuntime::PolylineBuilder*                       polylineBuilder = nullptr;
    Esri::ArcGISRuntime::Graphic*                               locationHistoryLineGraphic = nullptr;
    std::unique_ptr<Esri::ArcGISRuntime::GraphicsOverlay>       locationHistoryPointOverlay;
    std::unique_ptr<Esri::ArcGISRuntime::GraphicsOverlay>       locationHistoryLineOverlay;

    std::vector<QString> m_urlVectors;
    std::vector<QString> m_layerNames;
    std::vector<QCheckBox*> m_cBoxVectors;
    std::vector<Esri::ArcGISRuntime::Point> locations;
    Esri::ArcGISRuntime::Point lastPosition;

    int cBoxStateCurrent;
    QString m_leftPaneId;
    QString m_rightPaneId;
    double altitude;
    double heading;
    double latitude;
    double longitude;
    int counter{0};

    void setupViewPoint();
    void addMarker();
    void updateMarker(Esri::ArcGISRuntime::Point);
    void setLayersUrlVector();
    void createLayerMenu(std::vector<QString> &, std::vector<QString> &);
    void readGpsFromXplane();
    void setWindowsIds();
    void popupInformation();
    void displayLocationTrail();
};

#endif // COCKPIT_ARCGIS_H
