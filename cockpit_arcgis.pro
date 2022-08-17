#-------------------------------------------------
#  Copyright 2022 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TARGET = cockpit_arcgis
TEMPLATE = app

CONFIG += c++17

# additional modules are pulled in via arcgisruntime.pri
QT += widgets xml positioning gui
#LIBS += -L/usr/local/lib -lyaml-cpp

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 15) {
        error("$$TARGET requires Qt 5.15.2")
    }
	equals(QT_MINOR_VERSION, 15) : lessThan(QT_PATCH_VERSION, 2) {
		error("$$TARGET requires Qt 5.15.2")
	}
}

ARCGIS_RUNTIME_VERSION = 100.14.1
include($$PWD/arcgisruntime.pri)

win32:CONFIG += \
    embed_manifest_exe

SOURCES += \
    circle.cpp \
    coordinategrabber.cpp \
    main.cpp \
    Cockpit_arcgis.cpp \
    overlay.cpp \
    positionsourcesimulator.cpp \
    zmqreciever.cpp

HEADERS += \
    Cockpit_arcgis.h \
    circle.h \
    coordinategrabber.h \
    overlay.h \
    positionsourcesimulator.h \
    zmqreciever.h

LIBS += -L/usr/lib -lzmq
LIBS += -L/usr/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui

INCLUDEPATH += /usr/include/opencv4/
#-------------------------------------------------------------------------------

RESOURCES += \
    Cockpit_arcgis.qrc

DISTFILES +=

FORMS += \
    Cockpit_arcgis.ui
