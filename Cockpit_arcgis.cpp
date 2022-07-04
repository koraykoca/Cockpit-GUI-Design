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

// C++ API headers
#include "Basemap.h"

// Other headers
#include "Cockpit_arcgis.h"

#include <QDir>

#include "Map.h"
#include "MapGraphicsView.h"

#include "FeatureLayer.h"
#include "PictureMarkerSymbol.h"
#include "ServiceFeatureTable.h"
#include "CoordinateFormatter.h"

const QString homePath = QDir::homePath();
const QString yamlPath{"/dev/cpp/arcgis/cockpit_arcgis/param/gui_param_file.yaml"};
YAML::Node config = YAML::LoadFile((homePath + yamlPath).toStdString());
QString urlAddr = config["data_url"].as<QString>();
const QUrl dataUrl{urlAddr};

using namespace Esri::ArcGISRuntime;

cockpitArcgis::cockpitArcgis(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
{
    // Create a map using the ArcGISTopographic BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISTopographic, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // set the mapView as the central widget
    setCentralWidget(m_mapView);

    //create the action behaviours
    connect(m_mapView, SIGNAL(mouseClicked(QMouseEvent&)), this, SLOT(getCoordinate(QMouseEvent&)));

    // get location information
    // m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Navigation);
    m_mapView->locationDisplay()->start();

    // call the functions
    addLayer(dataUrl);
    setupViewPoint();
    addMarker();
}

// destructor
cockpitArcgis::~cockpitArcgis()
{
    m_mapView->locationDisplay()->stop();
}

/* class functions out-of-line definitions */

// focus on a specified area of the map with animation
void cockpitArcgis::setupViewPoint(){
    const Point center(11.35287, 48.06942, SpatialReference::wgs84());
    const Viewpoint view_point(center, 100000.0);
    m_mapView->setViewpointAnimated(view_point, 1.5, AnimationCurve::EaseInQuint);
}

// load vector layer and add it to the map
void cockpitArcgis::addLayer(QUrl path){
    std::unique_ptr<ServiceFeatureTable> ftrTable = std::make_unique<ServiceFeatureTable>(path, this);
    std::unique_ptr<FeatureLayer> ftrLayer = std::make_unique<FeatureLayer>(ftrTable.get(), this);
    m_mapView->setViewpointCenter(ftrLayer->fullExtent().center(), 80000);
    // m_map->operationalLayers()->clear();
    m_map->operationalLayers()->append(ftrLayer.get());
}

// add marker to a specific coordinate
void cockpitArcgis::addMarker(){
    Point startPoint;
    QVariantMap attr;
    attr["name"] = "Selected Coordinate";

    QImage icon{":/mapMarker.png"};  // get image from resources
    std::unique_ptr<PictureMarkerSymbol> marker = std::make_unique<PictureMarkerSymbol>(icon, this);
    marker->setOffsetY(12);

    std::unique_ptr<Graphic> graphicElement = std::make_unique<Graphic>(startPoint, attr, marker.get(), this);
    std::unique_ptr<GraphicsOverlay> graphicOverlay = std::make_unique<GraphicsOverlay>(this);
    graphicOverlay->graphics()->append(graphicElement.get());
    m_mapView->graphicsOverlays()->append(graphicOverlay.get());
}

// refresh marker position
void cockpitArcgis::updateMarker(Point newPoint){
    m_mapView->graphicsOverlays()->at(0)->graphics()->at(0)->setGeometry(newPoint);
}

// get coordinate of click
void cockpitArcgis::getCoordinate(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    mapPoint = CoordinateFormatter::fromLatitudeLongitude(mapCoordinates, SpatialReference::wgs84());
    updateMarker(std::move(mapPoint));
}
