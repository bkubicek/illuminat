#ifndef TRIFACE_H
#define TRIFACE_H
#include <math.h>
class TriFace
{
public:
    TriFace();
    float p[3][3];
    float n[3];
    float range[3][2];
    float a[3];
    float b[3];

    void calcRange();
    void calcN();
    bool isInZ(const float z);
    bool isInZ(const float zmin, const float zmax);

    float distanceFromArea(const float*);

    bool lineAtZ(const float z, /*line endpoints*/ float *a, float *b);
};

inline float scalar(const float *a,const float *b)
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

#endif // TRIFACE_H
