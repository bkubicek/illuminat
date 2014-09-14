#ifndef LAYER_H
#define LAYER_H

#include <vector>
#include "stlfile.h"
#include <QImage>

class Layer
{
public:
    Layer();
    int sx,sy; //sizex sizey
    std::vector<char> bvec;


    QImage toQImage();
    void fromStl(float z, StlFile &stl);

private:
    void bhm_line(int x1,int y1,int x2,int y2,char c); //line drawing
    bool flootFill(char fillcol, char bcol);

    static const char fchar=1;
    static const char fchar2=4;
    static const char bchar=0;
    static const char lchar=2;
};

#endif // LAYER_H
