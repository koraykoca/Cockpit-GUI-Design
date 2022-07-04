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
#include "Map.h"
#include "MapGraphicsView.h"
#include "FeatureLayer.h"
#include "PictureMarkerSymbol.h"
#include "ServiceFeatureTable.h"
#include "CoordinateFormatter.h"

// Other headers
#include "Cockpit_arcgis.h"
#include <QDir>
#include <QWindow>

const QString homePath = QDir::homePath();
const QString yamlPath{"/dev/cpp/arcgis/cockpit_arcgis/param/gui_param_file.yaml"};
YAML::Node config = YAML::LoadFile((homePath + yamlPath).toStdString());
QString urlAddr = config["data_url"].as<QString>();
const QUrl dataUrl{urlAddr};

using namespace Esri::ArcGISRuntime;

cockpitArcgis::cockpitArcgis(QWidget* parent /*=nullptr*/):
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    // Create a map using the ArcGISTopographic BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISTopographic, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);

    // Make this widget as parent
    ui->setupUi(this);

    // Make the app. fullscreen
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // Create layout for map
    layoutMap = std::make_unique<QVBoxLayout>();

    // Add map widget to the layout
    layoutMap->addWidget(m_mapView);

    // Remove margins for window borders
    layoutMap->setContentsMargins(0, 0, 0, 0);

    // Set the layout to the related frame in the GUI
    ui->mapFrame->setLayout(layoutMap.release());  // relinquish ownership to avoid double delete

    /* Integration of AirManager Panels */
    QWindow* leftPanelContainer = QWindow::fromWinId(0x2200002);
    QWidget* leftPanelWidget = QWidget::createWindowContainer(leftPanelContainer);

    QVBoxLayout* layoutLeftPanel = new QVBoxLayout();
    layoutLeftPanel->addWidget(leftPanelWidget);
    layoutLeftPanel->setContentsMargins(0, 0, 0, 0);
    ui->airManagerLeft->setLayout(layoutLeftPanel);

    QWindow* rightPanelContainer = QWindow::fromWinId(0x2600002);
    QWidget* rightPanelWidget = QWidget::createWindowContainer(rightPanelContainer);

    QVBoxLayout* layoutRightPanel = new QVBoxLayout();
    layoutRightPanel->addWidget(rightPanelWidget);
    layoutRightPanel->setContentsMargins(0, 0, 0, 0);
    ui->airManagerRight->setLayout(layoutRightPanel);

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
    delete ui;
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
