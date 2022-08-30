#include "layouts.h"
#include <QWindow>
#include <QWidget>
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

Layouts::Layouts(){   
    setWindowsIds();
}

// destructor
Layouts::~Layouts()
{
}

QVBoxLayout* Layouts::createlayoutLeftPanel(){
    // check if the ids get converted
    bool leftOk;
    long left = m_leftPaneId.toLong(&leftOk, 16);
    QVBoxLayout* layoutLeftPanel = new QVBoxLayout();
    QWindow* leftPanelContainer = QWindow::fromWinId(left);
    QWidget* leftPanelWidget = QWidget::createWindowContainer(leftPanelContainer);
    layoutLeftPanel->addWidget(leftPanelWidget);
    layoutLeftPanel->setContentsMargins(0, 0, 0, 0);
    return layoutLeftPanel;
}

QVBoxLayout* Layouts::createlayoutRightPanel(){
    // check if the ids get converted
    bool rightOk;
    long right = m_rightPaneId.toLong(&rightOk,16);
    QVBoxLayout* layoutRightPanel = new QVBoxLayout();
    QWindow* rightPanelContainer = QWindow::fromWinId(right);
    QWidget* rightPanelWidget = QWidget::createWindowContainer(rightPanelContainer);
    layoutRightPanel->addWidget(rightPanelWidget);
    layoutRightPanel->setContentsMargins(0, 0, 0, 0);
    return layoutRightPanel;
}


void Layouts::setWindowsIds()
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
