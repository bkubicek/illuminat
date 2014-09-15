#ifndef STLFILE_H
#define STLFILE_H

#include <vector>
#include "triface.h"
#include <QString>

class ZTree
{
 public:

    float split;

    ZTree *above, *below;
    std::vector<TriFace*> t;
    //insert(*TriFace)
};

class StlFile
{
public:
    StlFile();

    std::vector<TriFace> t;
    void read (const QString &filename);
    void write(const QString &filename, bool bin=true);

    float range[3][2];
    void calcRange();
    void calcBands(int n);

    std::vector<std::vector<int> > bands;
    std::vector<float> bandlimits;
    int findBand(const float z);
};

#endif // STLFILE_H
