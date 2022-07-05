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

#include "Cockpit.h"

#include "Basemap.h"
#include "Map.h"
#include "MapQuickView.h"
#include "FeatureLayer.h"
#include "PictureMarkerSymbol.h"
#include "ServiceFeatureTable.h"
#include "CoordinateFormatter.h"


#include <QUrl>
#include <QXmlStreamReader>
#include <QFile>
#include <QEvent>


using namespace Esri::ArcGISRuntime;

Cockpit::Cockpit(QObject* parent /* = nullptr */):
    QObject(parent),    m_map ( new Map(BasemapStyle::ArcGISTopographic, this))


{
    setLayersUrlVector();

}

Cockpit::~Cockpit()
{
    m_mapView->locationDisplay()->stop();
}

MapQuickView* Cockpit::mapView() const
{
    return m_mapView;
}

// Set the view (created in QML)
void Cockpit::setMapView(MapQuickView* mapView)
{
    if (!mapView || mapView == m_mapView)
    {
        return;
    }

    m_mapView = mapView;
    m_mapView->setMap(m_map);
    setupViewPoint();

    connect(m_mapView, SIGNAL(mouseClicked(QMouseEvent&)), this, SLOT(getCoordinates(QMouseEvent&)));
    m_mapView->locationDisplay()->start();
    addLayer(m_urlVectors[0]);
    addMarker();
    emit mapViewChanged();
}

// set the view, created in qml
void Cockpit::setupViewPoint() {

    const Point center(11.35287, 48.06942, SpatialReference::wgs84());
    const Viewpoint view_point(center, 100000.0);
    m_mapView->setViewpointAnimated(view_point, 1.5, AnimationCurve::EaseInQuint);
}

// load vector layer and add it to the map
void Cockpit::addLayer(QUrl path){
    std::unique_ptr<ServiceFeatureTable> ftrTable = std::make_unique<ServiceFeatureTable>(path, this);
    std::unique_ptr<FeatureLayer> ftrLayer = std::make_unique<FeatureLayer>(ftrTable.get(), this);
    m_mapView->setViewpointCenter(ftrLayer->fullExtent().center(), 80000);
    // m_map->operationalLayers()->clear();
    m_map->operationalLayers()->append(ftrLayer.get());
}

void Cockpit::addMarker(){
    Point startPoint;
    QVariantMap attr;
    attr["name"] = "Selected Coordinate";

    QImage icon{":/Resources/mapMarker.png"};  // get image from resources
    std::unique_ptr<PictureMarkerSymbol> marker = std::make_unique<PictureMarkerSymbol>(icon, this);
    marker->setOffsetY(12);

    std::unique_ptr<Graphic> graphicElement = std::make_unique<Graphic>(startPoint, attr, marker.get(), this);
    std::unique_ptr<GraphicsOverlay> graphicOverlay = std::make_unique<GraphicsOverlay>(this);
    graphicOverlay->graphics()->append(graphicElement.get());
    m_mapView->graphicsOverlays()->append(graphicOverlay.get());
}

// refresh marker position
void Cockpit::updateMarker(Point newPoint){
    m_mapView->graphicsOverlays()->at(0)->graphics()->at(0)->setGeometry(newPoint);
}

// get coordinate of click
void Cockpit::getCoordinates(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    mapPoint = CoordinateFormatter::fromLatitudeLongitude(mapCoordinates, SpatialReference::wgs84());
    updateMarker(std::move(mapPoint));
}

void Cockpit::setLayersUrlVector(){

    QFile xmlFile(":/Resources/guiParamFile.xml");

    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file" << xmlFile.errorString();
        exit(0);
    }

    QXmlStreamReader xmlReader(&xmlFile);

    if (xmlReader.readNextStartElement()){

        if (xmlReader.name() =="dataParameters") {

            while(xmlReader.readNextStartElement()) {
                if (xmlReader.name() == "data"){
                    while(xmlReader.readNextStartElement()){

                        if(xmlReader.name()== "dataUrl"){
                            QUrl u(xmlReader.readElementText());
                            m_urlVectors.push_back(u);
                        }
                        break;
                    }



                }
                else {
                    xmlReader.skipCurrentElement();
                }
            }

        }


    }
}
