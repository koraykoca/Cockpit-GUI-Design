#ifndef COORDINATEGRABBER_H
#define COORDINATEGRABBER_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <map>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QBoxLayout>
#include <QDebug>
#include <QDialog>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
class CoordinateGrabber
{
public:
    CoordinateGrabber();
    ~CoordinateGrabber();
    void setInstrumentCoordinatesFromImage();





    const std::map<QString, std::vector<int> > &instrumentNamesAndCoordinates() const;

private:
    void setCompareImagesNameFileMap();
    void setImageToCompareWith();
    void showImageInDialogUtility(cv::Mat img);

    cv::Mat m_imageToCompareWith;
    std::map<QString, cv::Mat> m_compareImagesNameFileMap;
    std::map<QString, std::vector<int>> m_instrumentNamesAndCoordinates;

    cv::Mat loadFromQrc(QString qrc, int flag = cv::IMREAD_COLOR);





};

#endif // COORDINATEGRABBER_H
