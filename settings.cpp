#include "settings.h"

Settings::Settings()
{
    exposure=0.001;
    layerheight=0.5;
    gcodeLayer=";layer";
    gcodeStart=";start";
    gcodeEnd=";end";
    printing=false;
    currentz=-1;
    stlLoaded=false;
    stlName="";
}
