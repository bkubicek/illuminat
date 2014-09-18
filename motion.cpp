#include "motion.h"
#include "settings.h"

#include <iostream>
using namespace std;

Motion::Motion( Settings *_set)
{
    set=_set;
}

void Motion::readSettingsFrom(QString filename)
{
    //filename;
}


void Motion::doLayerChange()
{
 send(set->gcodeLayer);
}

void Motion::doStart()
{
 send(set->gcodeStart);
}

void Motion::doEnd()
{
 send(set->gcodeEnd);
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
