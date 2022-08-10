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
#include "PictureMarkerSymbol.h"
#include "CoordinateFormatter.h"
#include "AbstractLocationDataSource.h"
#include "DefaultLocationDataSource.h"

#include <zmq.hpp>
#include<iostream>
#include <cstdlib>

#include "zmqreciever.h"
#include "positionsourcesimulator.h"
#include "overlay.h"
#include "circle.h"


using namespace Esri::ArcGISRuntime;

cockpitArcgis::cockpitArcgis(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    // Create a map using the ArcGISTopographic BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISNova, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // Make this widget as parent
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet("QWidget { background-color: gray; }");
    // Make the app. fullscreen
    // this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // Create layout for map
    layoutMap = std::make_unique<QVBoxLayout>();

    // Add map widget to the layout
    layoutMap->addWidget(m_mapView);

    // Remove margins for window borders
    layoutMap->setContentsMargins(0, 0, 0, 0);

    // Set the layout to the related frame in the GUI
    ui->mapFrame->setLayout(layoutMap.release());  // relinquish ownership to avoid double delete

    setWindowsIds();
    /* Integration of AirManager Panels */

    // check if the ids get converted
    bool leftOk;
    bool rightOk;

    long left = m_leftPaneId.toLong(&leftOk, 16);
    long right = m_rightPaneId.toLong(&rightOk,16);

    QWindow* leftPanelContainer = QWindow::fromWinId(left);
    leftPanelContainer->setFlag(Qt::FramelessWindowHint);
    m_leftPanelWidget = QWidget::createWindowContainer(leftPanelContainer);
    QVBoxLayout* layoutLeftPanel = new QVBoxLayout();
    layoutLeftPanel->addWidget(m_leftPanelWidget);
    layoutLeftPanel->setContentsMargins(0, 0, 0, 0);
    ui->airManagerLeft->setLayout(layoutLeftPanel);



    QWindow* rightPanelContainer = QWindow::fromWinId(right);
    rightPanelContainer->setFlag(Qt::FramelessWindowHint);
    m_rightPanelWidget = QWidget::createWindowContainer(rightPanelContainer);
    QVBoxLayout* layoutRightPanel = new QVBoxLayout();
    layoutRightPanel->addWidget(m_rightPanelWidget);
    layoutRightPanel->setContentsMargins(0, 0, 0, 0);
    ui->airManagerRight->setLayout(layoutRightPanel);
    //create the action behaviours
    connect(m_mapView, SIGNAL(mouseClicked(QMouseEvent&)), this, SLOT(getCoordinate(QMouseEvent&)));
    connect(m_mapView, SIGNAL(mouseMoved(QMouseEvent&)), this, SLOT(displayCoordinate(QMouseEvent&)));

    // get location information

    //   m_mapView->locationDisplay()->stop();

    ZmqReciever* zmqReciever = new ZmqReciever(this);

    m_positionSourceSimulator = new PositionSourceSimulator(zmqReciever, this);

    //m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::Navigation);
    m_mapView->locationDisplay()->start();
    m_mapView->locationDisplay()->setInitialZoomScale(100000);
    m_mapView->setZoomByPinchingEnabled(true);
    m_mapView->setRotationByPinchingEnabled(true);
  //  m_mapView->setViewpointScale(100);
   // m_mapView->locationDisplay()->setPositionSource(m_positionSourceSimulator); //Look for alternative later

    m_d = new DefaultLocationDataSource(this);
    m_d->setPositionInfoSource(m_positionSourceSimulator);
    m_d->start();
    m_mapView->locationDisplay()->setDataSource(m_d);
    m_mapView->locationDisplay()->setAutoPanMode(LocationDisplayAutoPanMode::CompassNavigation);


    // call the functions
    setLayersUrlVector();
    if (m_layerNames.size()){
        createLayerMenu(m_layerNames, m_urlVectors);
        connect(signalMapper, SIGNAL(mappedString(QString)), this, SLOT(arrangeLayers(QString)));
    }
    // setupViewPoint();
    addMarker();

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
    delete ui;
}

/* class functions out-of-line definitions */
// focus on a specified area of the map with animation
void cockpitArcgis::setupViewPoint(){


    const Point center(11.35287, 48.06942, SpatialReference::wgs84());
    const Viewpoint viewpoint(center, 100000.0);
    m_mapView->setViewpointAnimated(viewpoint, 1.5, AnimationCurve::EaseInQuint);
}

void cockpitArcgis::arrangeLayers(QString name){
    if (cBoxStateCurrent == 2){  // the item is checked.
        addLayer(QUrl(name));
    }
    if (cBoxStateCurrent == 0){  // the item is unchecked.
            for(auto &i : cBoxMap){
                if (i.second == QUrl(name)){
                    m_map->operationalLayers()->removeOne(i.first);
                    cBoxMap.erase(i.first);
                    break;
            }
        }
    }
}

// load vector layer and add it to the map
void cockpitArcgis::addLayer(QUrl path){
    ftrTable = new ServiceFeatureTable(path, this);
    ftrLayer = new FeatureLayer(ftrTable, this);
    //m_mapView->setViewpointCenter(ftrLayer->fullExtent().center(), 80000);
    m_map->operationalLayers()->append(ftrLayer);
    cBoxMap.insert(std::pair<FeatureLayer*,QUrl>(ftrLayer,path));
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


// display coordinate while hovering the mouse (keep pressing) over the map
void cockpitArcgis::displayCoordinate(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    ui->textCoordinate->setText(mapCoordinates);
}

// get coordinate of click
void cockpitArcgis::getCoordinate(QMouseEvent& event){
    Point mapPoint = m_mapView->screenToLocation(event.x(), event.y());
    auto mapCoordinates = CoordinateFormatter::toLatitudeLongitude(mapPoint, LatitudeLongitudeFormat::DecimalDegrees, 4);
    mapPoint = CoordinateFormatter::fromLatitudeLongitude(mapCoordinates, SpatialReference::wgs84());
    ui->textSelectedCoordinate->setText(mapCoordinates);
    updateMarker(std::move(mapPoint));
}

// construct layer menu
void cockpitArcgis::createLayerMenu(std::vector<QString>& name, std::vector<QString>& url){
    ui->layersToolButton->setPopupMode(QToolButton::InstantPopup);
    layerMenu = new QMenu();
    signalMapper = new QSignalMapper(this);
    for(unsigned long i=0 ; i<name.size(); i++){
        m_cBoxVectors.push_back(new QCheckBox(name[i]));
        connect(m_cBoxVectors[i], SIGNAL(stateChanged(int)), this, SLOT(getCBoxState(int)));
        connect(m_cBoxVectors[i], SIGNAL(stateChanged(int)), signalMapper, SLOT(map()));
        signalMapper->setMapping(m_cBoxVectors[i], url[i]);
        auto action = new QWidgetAction(m_cBoxVectors[i]);
        action->setDefaultWidget(m_cBoxVectors[i]);
        actionList.append(action);
    }
    layerMenu->addActions(actionList);
    ui->layersToolButton->setMenu(layerMenu);
}

void cockpitArcgis::getCBoxState(int state){
    cBoxStateCurrent = state;
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

void cockpitArcgis::setWindowsIds()
{
    QFile xmlFile(":/windowsId.xml");

    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file" << xmlFile.errorString();
        exit(0);
    }

    QXmlStreamReader xmlReader(&xmlFile);

    if (xmlReader.readNextStartElement()){
        while(xmlReader.readNextStartElement()){
            if (xmlReader.name() =="leftpane") {
                m_leftPaneId = xmlReader.readElementText();
            }

            else if (xmlReader.name() =="rightpane") {
                m_rightPaneId = xmlReader.readElementText();
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

void cockpitArcgis::closeEvent(QCloseEvent *e)
{

    m_leftPanelWidget->setParent(nullptr);
    m_leftPanelWidget->setWindowFlags(Qt::Window);
    m_leftPanelWidget->setWindowTitle("LeftPane");
    m_leftPanelWidget->show();

    m_rightPanelWidget->setParent(nullptr);
    m_rightPanelWidget->setWindowFlags(Qt::Window);
    m_rightPanelWidget->setWindowTitle("RightPane");
    m_rightPanelWidget->show();

    Q_UNUSED(e);
    QWidget::close();
}


