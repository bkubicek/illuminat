#include "motion.h"

#include <iostream>
using namespace std;

Motion::Motion()
{
    exposure=0.1;
    layerheight=0.5;
    gcodeLayer=";layer";
    gcodeStart=";start";
    gcodeEnd=";end";
}

void Motion::readSettingsFrom(QString filename)
{
    filename;
}


void Motion::doLayerChange()
{
 send(gcodeLayer);
}

void Motion::doStart()
{
 send(gcodeStart);
}

void Motion::doEnd()
{
 send(gcodeEnd);
}

void Motion::connect(QString port,int baud)
{
  #if QT_VERSION >= 0x050000
    serial.setPortName(port);
    serial.setBaudRate(baud);
   if (serial.open( QIODevice::ReadWrite))
   {


   }
   
  #endif
}
void Motion::disconnect()
{
  #if QT_VERSION >= 0x050000
    serial.close();
    
  #endif
}
void Motion::send(QString text)
{
  //serial.write(text.toLatin1());
    std::cout<<"Sending:"<<text.toStdString()<<std::endl;
}
