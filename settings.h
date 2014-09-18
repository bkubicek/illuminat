#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    Settings();

    QString gcodeStart;
    QString gcodeEnd;
    QString gcodeLayer;

    float exposure;
    float layerheight;

    bool printing;
    bool stlLoaded;
    QString stlName;

    float currentz;


};

#endif // SETTINGS_H
