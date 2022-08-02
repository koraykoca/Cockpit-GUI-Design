#include "coordinategrabber.h"

#include <chrono>
#include <iostream>
using namespace std::chrono;


CoordinateGrabber::CoordinateGrabber()
{
    setImageToCompareWith();
    setCompareImagesNameFileMap();
    setInstrumentCoordinatesFromImage();

}

CoordinateGrabber::~CoordinateGrabber()
{

}

void CoordinateGrabber::setInstrumentCoordinatesFromImage(){


    for (auto const& [name, image] : m_compareImagesNameFileMap){


        cv::Mat result, imgDisplay, templateImage;
        image.copyTo(templateImage);

        m_imageToCompareWith.copyTo(imgDisplay);

        int result_cols = imgDisplay.cols - templateImage.cols + 1;
        int result_rows = imgDisplay.rows - templateImage.rows + 1;
        result.create( result_rows, result_cols, CV_32FC1 );

        auto start = high_resolution_clock::now();

        cv::matchTemplate(imgDisplay,templateImage, result, cv::TM_SQDIFF );
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        cv::normalize(result, result, 0,1, cv::NORM_MINMAX, -1, cv::Mat());

        std::cout << "Duration in miliseconds " <<duration.count() << std::endl;
        double minVal, maxVal;
        cv::Point minLoc, maxLoc, matchLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

        matchLoc = minLoc; // This is different based on matching algorithm used, refer to the docs for more information https://docs.opencv.org/3.4/de/da9/tutorial_template_matching.html
        cv::Point oppositeEnd = cv::Point( matchLoc.x + templateImage.cols , matchLoc.y + templateImage.rows );
        rectangle( imgDisplay, matchLoc, oppositeEnd, cv::Scalar::all(255), 2, 8, 0 );
        std::vector<int> coordinates = {matchLoc.x, matchLoc.y, oppositeEnd.x, oppositeEnd.y };

        m_instrumentNamesAndCoordinates.insert({name,coordinates});




//        showImageInDialogUtility(imgDisplay);


    }




}

void CoordinateGrabber::setCompareImagesNameFileMap()
{
    QString directoryPath = ":/images_for_coordinates";
    QDir directory(directoryPath);

    QStringList imagesList = directory.entryList((QStringList("*.png")));

    for (auto& imagePath: imagesList){




        QString qrcPath = directoryPath+ "/"+imagePath;
        cv::Mat img = loadFromQrc(qrcPath, 1);
        QStringList list = imagePath.split('.');
        QString fileName = list[0];
        m_compareImagesNameFileMap.insert({fileName, img});
//        showImageInDialogUtility(img);

    }


}

void CoordinateGrabber::setImageToCompareWith()
{
    m_imageToCompareWith = loadFromQrc(":/windows.png",1 );
//    showImageInDialogUtility(m_imageToCompareWith);

}

void CoordinateGrabber::showImageInDialogUtility(cv::Mat img)
{
            QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
            QDialog dlg;
            QHBoxLayout *l = new QHBoxLayout(&dlg);
            QLabel *label = new QLabel();
            l->addWidget(label);
            label->setPixmap(QPixmap::fromImage(imdisplay));
            dlg.setGeometry(30,30, 100,100);
            dlg.exec();

}

const std::map<QString, std::vector<int> > &CoordinateGrabber::instrumentNamesAndCoordinates() const
{
    return m_instrumentNamesAndCoordinates;
}

cv::Mat CoordinateGrabber::loadFromQrc(QString qrc, int flag)
{
    QFile file(qrc);
    cv::Mat brgImage,grayImage;
    if(file.open(QIODevice::ReadOnly))
    {
        qint64 sz = file.size();
        std::vector<uchar> buf(sz);
        file.read((char*)buf.data(), sz);
        brgImage = cv::imdecode(buf, flag);
        cv::cvtColor(brgImage,grayImage, cv::COLOR_BGR2GRAY);
        cv::normalize(grayImage, grayImage, 0., 255., cv::NORM_MINMAX, CV_8U);
    }
    return grayImage;

}
