#ifndef LAYOUTS_H
#define LAYOUTS_H

#include <QString>
#include <QVBoxLayout>

class Layouts
{
friend class cockpitArcgis;

public:
    explicit Layouts();
    ~Layouts();

private:
    QString m_leftPaneId;
    QString m_rightPaneId;

    void setWindowsIds();
    QVBoxLayout* createlayoutLeftPanel();
    QVBoxLayout* createlayoutRightPanel();
};

#endif // LAYOUTS_H
