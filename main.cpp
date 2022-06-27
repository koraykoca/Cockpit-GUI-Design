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

// Qt headers
#include <QApplication>
#include <QMessageBox>


#include "ArcGISRuntimeEnvironment.h"

#include "Cockpit_arcgis.h"

using namespace Esri::ArcGISRuntime;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication application(argc, argv);

    // Use of Esri location services, including basemaps and geocoding, requires
    // either an ArcGIS identity or an API key. For more information see
    // https://links.esri.com/arcgis-runtime-security-auth.

    // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
    // organization in ArcGIS Online or ArcGIS Enterprise.

    // 2. API key: A permanent key that gives your application access to Esri
    // location services. Create a new API key or access existing API keys from
    // your ArcGIS for Developers dashboard (https://links.esri.com/arcgis-api-keys).

    const QString apiKey = QString("AAPK79d58ed494ae4429a9f3968e7fa31f7fACrrkeKHSRryqIVzCc2b-TD-oRFFam9WM5ethP9y0J4SF1E21ugsyk6k9jrmipy5");
    if (apiKey.isEmpty())
    {
        qWarning() << "Use of Esri location services, including basemaps, requires" <<
                      "you to authenticate with an ArcGIS identity or set the API Key property.";
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }

    // Production deployment of applications built with ArcGIS Runtime requires you to
    // license ArcGIS Runtime functionality. For more information see
    // https://links.esri.com/arcgis-runtime-license-and-deploy.

    // ArcGISRuntimeEnvironment::setLicense("Place license string in here");

    //  use this code to check for initialization errors
    //  QObject::connect(ArcGISRuntimeEnvironment::instance(), &ArcGISRuntimeEnvironment::errorOccurred, [](const Error& error){
    //    QMessageBox msgBox;
    //    msgBox.setText(error.message);
    //    msgBox.exec();
    //  });

    //  if (ArcGISRuntimeEnvironment::initialize() == false)
    //  {
    //    application.quit();
    //    return 1;
    //  }

    cockpitArcgis applicationWindow;
    applicationWindow.setMinimumWidth(800);
    applicationWindow.setMinimumHeight(600);
    applicationWindow.show();

    return application.exec();
}
