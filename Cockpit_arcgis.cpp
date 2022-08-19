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
#include <QXmlStreamReader>
#include <QFile>
#include <QWindow>
#include <QWidgetAction>
#include <QGeoPositionInfo>

#include "Map.h"
#include "MapGraphicsView.h"
#include "FeatureLayer.h"
#include "CoordinateFormatter.h"
#include "AbstractLocationDataSource.h"
#include "DefaultLocationDataSource.h"
#include "SimpleRenderer.h"

#include <zmq.hpp>
#include<iostream>
#include <cstdlib>

#include "zmqreciever.h"
#include "positionsourcesimulator.h"
#include "overlay.h"
#include "circle.h"
#include "layouts.h"

using namespace Esri::ArcGISRuntime;

cockpitArcgis::cockpitArcgis(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
  , m_ui(new Ui::MainWindow)
{
    m_map = new Map(BasemapStyle::ArcGISNova, this);  // set ArcGISNova Basemap as default

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // Make this widget as parent
    m_ui->setupUi(this);
    m_ui->centralwidget->setStyleSheet("QWidget { background-color: gray; }");
    // Make the app. fullscreen
    // this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // Create layout for map
    m_layoutMap = std::make_unique<QVBoxLayout>();

    // Add map widget to the layout
    m_layoutMap->addWidget(m_mapView);

    // Remove margins for window borders
    m_layoutMap->setContentsMargins(0, 0, 0, 0);

    // Set the layout to the related frame in the GUI
    m_ui->mapFrame->setLayout(m_layoutMap.release());  // relinquish ownership to avoid double delete

    /* Integration of AirManager Panels */
    Layouts layouts = Layouts();
    m_layoutLeftPanel = layouts.createlayoutLeftPanel();
    m_layoutRightPanel = layouts.createlayoutRightPanel();
    m_ui->airManagerLeft->setLayout(m_layoutLeftPanel);
    m_ui->airManagerRight->setLayout(m_layoutRightPanel);

    //create the action behaviours
    connect(m_mapView, SIGNAL(mouseClicked(QMouseEvent&)), this, SLOT(getCoordinate(QMouseEvent&)));
    connect(m_mapView, SIGNAL(mouseMoved(QMouseEvent&)), this, SLOT(displayCoordinate(QMouseEvent&)));

    // get location information
    ZmqReciever* zmqReciever = new ZmqReciever(this);
    m_positionSourceSimulator = new PositionSourceSimulator(zmqReciever, this);
    connect(zmqReciever, &ZmqReciever::gpsPositionChanged, this, &cockpitArcgis::updatesFromZmq);

    m_d = new DefaultLocationDataSource(this);
    m_d->setPositionInfoSource(m_positionSourceSimulator);
    m_planeIcon = new QImage(":/planeIcon.png");
    m_planeMarker = std::make_unique<PictureMarkerSymbol>(*m_planeIcon, this);
    m_mapView->locationDisplay()->setDefaultSymbol(m_planeMarker.get());
    m_mapView->locationDisplay()->setDataSource(m_d);
    m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Recenter);
    m_mapView->locationDisplay()->setInitialZoomScale(100000);
    m_mapView->setZoomByPinchingEnabled(true);
    m_mapView->setRotationByPinchingEnabled(true);
    // m_mapView->setViewpointScale(100);
    // m_mapView->locationDisplay()->setPositionSource(m_positionSourceSimulator);  //ToDo: Look for alternative later
    m_mapView->locationDisplay()->start();
    m_d->start();

    // call the functions
    setLayersUrlVector();
    if (m_layerNames.size()){
        createLayerMenu(m_layerNames, m_urlVectors);
        connect(m_layerSignalMapper, SIGNAL(mappedString(QString)), this, SLOT(arrangeLayers(QString)));
    }
    // setupViewPoint();
    addMarker();
    //popupInformation();

    displayLocationTrail();

    createBaseMapMenu();
    connect(m_mapSignalMapper, SIGNAL(mappedInt(int)), this, SLOT(setBaseMap(int)));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &cockpitArcgis::detectTimeout);
    timer->start(100);
    connect(this, SIGNAL(timeoutDetection(bool)), this, SLOT(addNewPolyline(bool)));

    Circle* c = new Circle();
//    c->setParent(ui->centralwidget);
    c->move(450,700);
    c->show();

//    overlay* m_overlay = new overlay(this);
//    m_overlay->setParent(ui->centralwidget);
//    m_overlay->resize(ui->menuFrame->size());
//    m_overlay->move(50,50);
//    m_overlay->show();

}

// destructor
cockpitArcgis::~cockpitArcgis()
{
    m_mapView->locationDisplay()->stop();
    delete m_ui;
    delete m_layoutLeftPanel;
    delete m_layoutRightPanel;
}

/* class functions out-of-line definitions */
// focus on a specified area of the map with animation
void cockpitArcgis::setupViewPoint(){
    const Point center(11.35287, 48.06942, SpatialReference::wgs84());
    const Viewpoint viewpoint(center, 100000.0);
    m_mapView->setViewpointAnimated(viewpoint, 1.5, AnimationCurve::EaseInQuint);
}

void cockpitArcgis::arrangeLayers(QString name){
    if (m_layerCBoxStateCurrent == 2){  // the item is checked.
        addLayer(QUrl(name));
    }
    if (m_layerCBoxStateCurrent == 0){  // the item is unchecked.
            for(auto &i : m_cBoxMap){
                if (i.second == QUrl(name)){
                    m_map->operationalLayers()->removeOne(i.first);
                    m_cBoxMap.erase(i.first);
                    break;m_layerMenu = new QMenu();
            }
        }
    }
}

// load vector layer and add it to the map
void cockpitArcgis::addLayer(QUrl path){
    m_ftrTable = new ServiceFeatureTable(path, this);
    m_ftrLayer = new FeatureLayer(m_ftrTable, this);
    //m_mapView->setViewpointCenter(ftrLayer->fullExtent().center(), 80000);
    m_map->operationalLayers()->append(m_ftrLayer);
    m_cBoxMap.insert(std::pair<FeatureLayer*,QUrl>(m_ftrLayer,path));
}

// add marker to the map
void cockpitArcgis::addMarker(){
    Point startPoint;
    QVariantMap attr;
    attr["name"] = "Selected Coordinate";

    QImage icon{":/mapMarker.png"};  // get image from resources
    std::unique_ptr<PictureMarkerSymbol> marker = std::make_unique<PictureMarkerSymbol>(icon, this);
    marker->setOffsetY(12);  // put the tip of the marker on where clicked

    std::unique_ptr<Graphic> graphicElement = std::make_unique<Graphic>(startPoint, attr, marker.get(), this);
    std::unique_ptr<GraphicsOverlay> graphicOverlay = std::make_unique<GraphicsOverlay>(this);
    graphicOverlay->graphics()->append(graphicElement.get());
    m_mapView->graphicsOverlays()->append(graphicOverlay.get());
}

// move the marker to a new position
void cockpitArcgis::updateMarker(Point newPoint){
    m_mapView->graphicsOverlays()->at(0)->graphics()->at(0)->setGeometry(newPoint);
}

// display coordinate while hovering the mouse (keep pressing) over the map
void cockpitArcgis::displayCoordinate(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    m_ui->textCoordinate->setText(mapCoordinates);
}

// get coordinate of click
void cockpitArcgis::getCoordinate(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    mapPoint = CoordinateFormatter::fromLatitudeLongitude(mapCoordinates, SpatialReference::wgs84());
    m_ui->textSelectedCoordinate->setText(mapCoordinates);
    updateMarker(std::move(mapPoint));
}

// construct layer menu
void cockpitArcgis::createLayerMenu(std::vector<QString>& name, std::vector<QString>& url){
    m_ui->layersToolButton->setPopupMode(QToolButton::InstantPopup);
    m_layerMenu = new QMenu();
    m_layerSignalMapper = new QSignalMapper(this);
    for(unsigned long i=0 ; i<name.size(); i++){
        m_cBoxVectors.push_back(new QCheckBox(name[i]));
        connect(m_cBoxVectors[i], SIGNAL(stateChanged(int)), this, SLOT(getLayerCBoxState(int)));
        connect(m_cBoxVectors[i], SIGNAL(stateChanged(int)), m_layerSignalMapper, SLOT(map()));
        m_layerSignalMapper->setMapping(m_cBoxVectors[i], url[i]);
        auto action = new QWidgetAction(m_cBoxVectors[i]);
        action->setDefaultWidget(m_cBoxVectors[i]);
        m_actionList.append(action);
    }
    m_layerMenu->addActions(m_actionList);
    m_ui->layersToolButton->setMenu(m_layerMenu);
}

void cockpitArcgis::getLayerCBoxState(int state){
    m_layerCBoxStateCurrent = state;
}

void cockpitArcgis::getMapCBoxState(int state){
    m_mapCBoxStateCurrent = state;
}

// add popup information next to the plane
void cockpitArcgis::popupInformation(){
    Point initPoint{13.3227, 47.4882};
    TextSymbol* textSymbol = new TextSymbol("TESSSTT", QColor(Qt::darkBlue), 24.0, HorizontalAlignment::Right, VerticalAlignment::Middle, m_mapView);
    textSymbol->setBackgroundColor(QColor(Qt::darkYellow));
    std::unique_ptr<Graphic> textGraphic = std::make_unique<Graphic>(initPoint, textSymbol, m_mapView);
    std::unique_ptr<GraphicsOverlay> graphicOverlay2 = std::make_unique<GraphicsOverlay>(m_mapView);
    graphicOverlay2->graphics()->append(textGraphic.get());
    m_mapView->graphicsOverlays()->append(graphicOverlay2.get());
    m_mapView->graphicsOverlays()->at(1)->graphics()->at(0)->setGeometry(initPoint);
}

// display location trails on the map
void cockpitArcgis::displayLocationTrail(){
    // overlays to display location trails
    m_locationHistoryPointOverlay = new GraphicsOverlay(m_mapView);
    GraphicsOverlay* locationHistoryLineOverlay = new GraphicsOverlay(m_mapView);
    m_mapView->graphicsOverlays()->append(m_locationHistoryPointOverlay);
    m_mapView->graphicsOverlays()->append(locationHistoryLineOverlay);

    // graphics overlay for displaying the trail
    m_locationLineSymbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor::fromRgb(QRandomGenerator::global()->generate()), 2, this);
    locationHistoryLineOverlay->setRenderer(new SimpleRenderer(m_locationLineSymbol, this));
    m_locationHistoryLineGraphic = new Graphic(this);
    locationHistoryLineOverlay->graphics()->append(m_locationHistoryLineGraphic);

    // line symbols are used to display graphics and features which are based on polyline geometries
    m_polylineBuilder = new PolylineBuilder(SpatialReference::wgs84(), this);

    // graphics overlay for showing points
    m_locationPointSymbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor::fromRgb(QRandomGenerator::global()->generate()), 3, this);
    m_locationHistoryPointOverlay->setRenderer(new SimpleRenderer(m_locationPointSymbol, this));

    connect(m_mapView->locationDisplay(), &LocationDisplay::locationChanged, this, [this](const Location& location)
    {
        m_counter++;

        // clear old route
        m_locationHistoryLineGraphic->setGeometry(Geometry());

        if (m_lastPosition.isValid() && m_counter == 15)
        {
          m_polylineBuilder->addPoint(m_lastPosition);
          m_locationHistoryPointOverlay->graphics()->append(new Graphic(m_lastPosition, this));
          m_counter = 0;
        }

        // store the current position
        m_lastPosition = location.position();

        // update the polyline
        m_locationHistoryLineGraphic->setGeometry(m_polylineBuilder->toGeometry());
    });
}

void cockpitArcgis::detectTimeout(){
     // emits true when location being displayed is based on the last known position, and the new position is still being acquired
     bool timeout = m_mapView->locationDisplay()->location().isLastKnown();
     emit timeoutDetection(timeout);
}

void cockpitArcgis::addNewPolyline(const bool timeout){
    if (timeout){
        if (!m_polylineBuilder->isEmpty()){
            displayLocationTrail();
        }
    }
}

void cockpitArcgis::updatesFromZmq(QVector<double> newAttributes){
    m_latitude = newAttributes[0];           // deg
    m_longitude = newAttributes[1];          // deg
    m_altitude = newAttributes[2];           // m above MSL
    m_heading = newAttributes[5];            // deg
    QImage transformed_planeIcon = m_planeIcon->transformed(QTransform().rotate(m_heading), Qt::SmoothTransformation);
    m_planeMarker = std::make_unique<PictureMarkerSymbol>(transformed_planeIcon, this);
    m_mapView->locationDisplay()->setDefaultSymbol(m_planeMarker.get());
    // m_mapView->setViewpointRotation(heading);  // rotate the map itself
}

// construct layer menu
void cockpitArcgis::createBaseMapMenu(){
    m_ui->panModeButton->setPopupMode(QToolButton::InstantPopup);
    QMenu* baseMapsMenu = new QMenu();
    QCheckBox* CBoxTopographic = new QCheckBox("Topographic");
    QCheckBox* CBoxNova = new QCheckBox("Nova");
    m_mapSignalMapper = new QSignalMapper(this);
    connect(CBoxTopographic, SIGNAL(stateChanged(int)), this, SLOT(getMapCBoxState(int)));
    connect(CBoxTopographic, SIGNAL(stateChanged(int)), m_mapSignalMapper, SLOT(map()));
    m_mapSignalMapper->setMapping(CBoxTopographic, 14);
    connect(CBoxNova, SIGNAL(stateChanged(int)), this, SLOT(getMapCBoxState(int)));
    connect(CBoxNova, SIGNAL(stateChanged(int)), m_mapSignalMapper, SLOT(map()));
    m_mapSignalMapper->setMapping(CBoxNova, 24);
    auto actionTopographic = new QWidgetAction(CBoxTopographic);
    auto actionNova = new QWidgetAction(CBoxNova);
    actionTopographic->setDefaultWidget(CBoxTopographic);
    actionNova->setDefaultWidget(CBoxNova);
    baseMapsMenu->addAction(actionTopographic);
    baseMapsMenu->addAction(actionNova);
    m_ui->panModeButton->setMenu(baseMapsMenu);
}

void cockpitArcgis::setBaseMap(int style){
    if (m_mapCBoxStateCurrent == 2){  // the item is checked.
        switch (style){
            case 14:
                m_basemap = new Basemap(BasemapStyle::ArcGISTopographic, this);
            break;
            case 24:
                m_basemap = new Basemap(BasemapStyle::ArcGISNova, this);
        }
        m_map->setBasemap(m_basemap);
        m_mapView->setMap(m_map);
    }
}

// read layer data from XML file
void cockpitArcgis::setLayersUrlVector(){

    QFile xmlFile(":/guiParamFile.xml");

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
                        if(xmlReader.name()== "url"){
                            QString u(xmlReader.readElementText());
                            m_urlVectors.push_back(u);
                        }
                        if(xmlReader.name()== "name"){
                            QString n(xmlReader.readElementText());
                            m_layerNames.push_back(n);
                        }
                    }
                }
                else {
                    xmlReader.skipCurrentElement();
                }
            }
        }
    }
}

void cockpitArcgis::planeGpsPositionChanged(QVector<double> newLocation){

    double lat = newLocation[0];
    double lon = newLocation[1];
    Point point(lat, lon, SpatialReference::wgs84());
    Location location(point, 10,10,10,10); //random values

    qDebug()<<"Location changed";
    //    const Viewpoint viewpoint(point,0 ,100000.0);
    //    m_mapView->setViewpointCenter(point);
    QGeoCoordinate c(lat, lon);
    QGeoPositionInfo currentPosition;
    currentPosition.setCoordinate(c);
    //  emit positionUpdated(currentPosition);

}
