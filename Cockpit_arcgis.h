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
}
}

#include <QMainWindow>
#include "Point.h"

extern const QUrl data_path;

class cockpitArcgis : public QMainWindow
{
    Q_OBJECT
public:
    explicit cockpitArcgis(QWidget* parent = nullptr);
    ~cockpitArcgis() override;

public slots:
    void get_coordinate(QMouseEvent&);

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;

    void setup_view_point();
    void add_layer(QUrl);
    void add_marker();
    void update_marker(Esri::ArcGISRuntime::Point);
};

#endif // COCKPIT_ARCGIS_H
