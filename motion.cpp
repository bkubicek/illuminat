#include "motion.h"

#include <iostream>
using namespace std;

Motion::Motion()
{
    exposure=0.5;
    layerheight=0.1;
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
    serial.setPortName(port);
    serial.setBaudRate(baud);
   if (serial.open( QIODevice::ReadWrite))
   {


   }
}
void Motion::disconnect()
{
    serial.close();
}
void Motion::send(QString text)
{
  //serial.write(text.toLatin1());
    std::cout<<"Sending:"<<text.toStdString()<<std::endl;
}
