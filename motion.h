#ifndef MOTION_H
#define MOTION_H
#include <QString>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class Motion
{
public:
    Motion();
    void readSettingsFrom(QString filename);
    void doLayerChange();
    void doStart();
    void doEnd();

    QString gcodeStart;
    QString gcodeEnd;
    QString gcodeLayer;

    float exposure;
    float layerheight;


    void connect(QString port,int baud);
    void disconnect();
    void send(QString text);
    QSerialPort serial;
};

#endif // MOTION_H
