#ifndef MOTION_H
#define MOTION_H
#include <QString>


#if QT_VERSION >= 0x050000
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#endif


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
    #if QT_VERSION >= 0x050000
    QSerialPort serial;
    #else
    
    #endif
    
};

#endif // MOTION_H
