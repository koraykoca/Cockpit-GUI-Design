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

extern const QUrl dataPath;

class Cockpit_arcgis : public QMainWindow
{
    Q_OBJECT
public:
    explicit Cockpit_arcgis(QWidget* parent = nullptr);
    ~Cockpit_arcgis() override;

public slots:

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;

    void setupViewpoint();
    void displayShapeFile(QUrl);
    void addGeometry();
};

#endif // COCKPIT_ARCGIS_H
