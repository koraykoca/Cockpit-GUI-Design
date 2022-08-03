#include "zmqreciever.h"
#include <thread>
#include <zmq.hpp>
#include <iostream>
#include <string>
#include <QDebug>

ZmqReciever::ZmqReciever(QObject *parent)
    : QObject{parent}
{
    start();
}

void ZmqReciever::start(){
    std::thread (&ZmqReciever::execute,this).detach();
}

void ZmqReciever::execute (){
    std::string connectString ("tcp://137.193.83.173:5555");  // set it to the IP adress of the machine which sends the data, default: 127.0.0.1
    zmq::context_t context(1);
    zmq::socket_t pullSocket(context, zmq::socket_type::pull);
    pullSocket.connect(connectString);


    while(true)
    {
        // send the request message

        zmq::message_t gpsPosition;
        pullSocket.recv(gpsPosition, zmq::recv_flags::none);

        // qDebug()<< QString::fromStdString(gpsPosition.to_string());
        QString gpsString(QString::fromStdString(gpsPosition.to_string()));
        gpsString.remove(" ");


        QStringList stringList = gpsString.split(",");

        QVector<double> gpsPositions;

        for (const QString &str: qAsConst(stringList)){
            gpsPositions.push_back(str.toFloat());
        }
        qDebug() <<gpsPositions;
        emit gpsPositionChanged(gpsPositions);

    }

}
