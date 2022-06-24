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

#include <QUrl>

#include "Map.h"
#include "MapGraphicsView.h"

#include <FeatureLayer.h>
#include <PictureMarkerSymbol.h>
#include <ServiceFeatureTable.h>

const QUrl dataPath{"https://services-eu1.arcgis.com/jqApksCDxo9OIytM/arcgis/rest/services/vector_daten/FeatureServer/12"};

using namespace Esri::ArcGISRuntime;

Cockpit_arcgis::Cockpit_arcgis(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
{
    // Create a map using the ArcGISTopographic BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISTopographic, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Navigation);
    m_mapView->locationDisplay()->start();

    // set the mapView as the central widget
    setCentralWidget(m_mapView);

    displayShapeFile(dataPath);
    addGeometry();
    setupViewpoint();
}

// destructor
Cockpit_arcgis::~Cockpit_arcgis()
{
}

// focus on a specified area of the map with animation
void Cockpit_arcgis::setupViewpoint(){
    const Point center(11.35287, 48.06942, SpatialReference::wgs84());
    const Viewpoint viewpoint(center, 100000.0);
    m_mapView->setViewpointAnimated(viewpoint, 1.5, AnimationCurve::EaseInQuint);
}

void Cockpit_arcgis::displayShapeFile(QUrl path){
    //std::unique_ptr<ShapefileFeatureTable> ftr_table = std::make_unique<ShapefileFeatureTable>(path, this);
    std::unique_ptr<ServiceFeatureTable> ftr_table = std::make_unique<ServiceFeatureTable>(path, this);
    std::unique_ptr<FeatureLayer> ftr_layer = std::make_unique<FeatureLayer>(ftr_table.get(), this);
    m_mapView->setViewpointCenter(ftr_layer->fullExtent().center(), 80000);
    // m_map->operationalLayers()->clear();
    m_map->operationalLayers()->append(ftr_layer.get());
}

void Cockpit_arcgis::addGeometry(){
    Point coordPoint{11.38043, 48.06679, SpatialReference::wgs84()};
    QVariantMap attr;
    attr["name"] = "Selected Coordinate";

    // const QString homePath = QDir::homePath();
    // QString iconsPath{"/dev/cpp/arcgis/icons/mapMarker.png"};
    // QImage icon{homePath + iconsPath};
    QUrl icon_url{"https://raw.githubusercontent.com/koraykoca/GUI-GIS/main/mapMarker.png"};
    std::unique_ptr<PictureMarkerSymbol> marker = std::make_unique<PictureMarkerSymbol>(icon_url, this);
    marker->setOffsetY(12);

    std::unique_ptr<Graphic> graphic_element = std::make_unique<Graphic>(coordPoint, attr, marker.get(), this);
    std::unique_ptr<GraphicsOverlay> graphic_overlay = std::make_unique<GraphicsOverlay>(this);
    graphic_overlay->graphics()->append(graphic_element.get());
    m_mapView->graphicsOverlays()->append(graphic_overlay.get());
}
