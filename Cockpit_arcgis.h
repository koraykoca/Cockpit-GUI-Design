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
#include "Basemap.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleRenderer.h"

class cockpitArcgis : public QMainWindow
{
    Q_OBJECT
public:
    explicit cockpitArcgis(QWidget* parent = nullptr);
    ~cockpitArcgis() override;

signals:
    void timeoutDetection(const bool result);

public slots:
    void getCoordinate(QMouseEvent&);
    void displayCoordinate(QMouseEvent&);
    void addLayer(QUrl);
    void arrangeLayers(QString);
    void getLayerCBoxState(int);
    void getMapCBoxState(int);
    void planeGpsPositionChanged(QVector<  double> newPosition);
    void updatesFromZmq(QVector<double> newAttributes);
    void setBaseMap(int);
    void addNewPolyline(const bool);
    void detectTimeout();

private:
    Esri::ArcGISRuntime::Map*                                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*                       m_mapView = nullptr;
    Esri::ArcGISRuntime::DefaultLocationDataSource*             m_d = nullptr;
    Ui::MainWindow*                                             m_ui;
    std::unique_ptr<QVBoxLayout>                                m_layoutMap;
    QMenu*                                                      m_layerMenu;
    QList<QAction*>                                             m_actionList;
    QSignalMapper*                                              m_layerSignalMapper;
    QSignalMapper*                                              m_mapSignalMapper;
    Esri::ArcGISRuntime::ServiceFeatureTable*                   m_ftrTable;
    Esri::ArcGISRuntime::FeatureLayer*                          m_ftrLayer;
    PositionSourceSimulator*                                    m_positionSourceSimulator = nullptr;
    std::map<Esri::ArcGISRuntime::FeatureLayer*, QUrl>          m_cBoxMap;
    std::unique_ptr<Esri::ArcGISRuntime::PictureMarkerSymbol>   m_planeMarker;
    QImage*                                                     m_planeIcon;
    Esri::ArcGISRuntime::PolylineBuilder*                       m_polylineBuilder = nullptr;
    Esri::ArcGISRuntime::Graphic*                               m_locationHistoryLineGraphic = nullptr;
    Esri::ArcGISRuntime::Basemap*                               m_basemap;
    Esri::ArcGISRuntime::SimpleLineSymbol*                      m_locationLineSymbol;
    Esri::ArcGISRuntime::SimpleMarkerSymbol*                    m_locationPointSymbol;
    Esri::ArcGISRuntime::GraphicsOverlay*                       m_locationHistoryPointOverlay;
    QVBoxLayout*                                                m_layoutLeftPanel;
    QVBoxLayout*                                                m_layoutRightPanel;
    Esri::ArcGISRuntime::SimpleRenderer*                        m_rendererLine;
    Esri::ArcGISRuntime::SimpleRenderer*                        m_rendererPoint;

    std::vector<QString> m_urlVectors;
    std::vector<QString> m_layerNames;
    std::vector<QCheckBox*> m_cBoxVectors;
    std::vector<Esri::ArcGISRuntime::Point> m_locations;
    Esri::ArcGISRuntime::Point m_lastPosition;

    int m_layerCBoxStateCurrent;
    int m_mapCBoxStateCurrent;
    double m_altitude;
    double m_heading;
    double m_latitude;
    double m_longitude;
    int m_counter{0};
    int m_counter2{0};

    void setupViewPoint();
    void addMarker();
    void updateMarker(Esri::ArcGISRuntime::Point);
    void setLayersUrlVector();
    void createLayerMenu(std::vector<QString> &, std::vector<QString> &);
    void readGpsFromXplane();
    void setWindowsIds();
    void popupInformation();
    void displayLocationTrail();
    void createBaseMapMenu();
};

#endif // COCKPIT_ARCGIS_H
