#ifndef MOTION_H
#define MOTION_H
#include <QString>


#if QT_VERSION >= 0x050000
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#endif

class Settings;

class Motion
{
public:
    Motion(Settings *_set);
    void readSettingsFrom(QString filename);
    void doLayerChange();
    void doStart();
    void doEnd();



    void connect(QString port,int baud);
    void disconnect();
    void send(QString text);

    Settings *set;
    #if QT_VERSION >= 0x050000
    QSerialPort serial;
    #else
    
    #endif
    
};

#endif // MOTION_H
