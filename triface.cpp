#include "triface.h"
#include <float.h>
#include <iostream>
using namespace std;
TriFace::TriFace()
{
    for (int i=0;i<3;i++)
    {
        n[i]=0;
        range[i][0]=0;range[i][1]=0;
        p[i][0]=0;p[i][1]=0;p[i][2]=0;
    }

}

void TriFace::calcRange()
{
    range[0][0]=FLT_MAX;range[0][1]=FLT_MIN;
    range[1][0]=FLT_MAX;range[1][1]=FLT_MIN;
    range[2][0]=FLT_MAX;range[2][1]=FLT_MIN;
    for(int i=0;i<3;i++)
    {
         for(int j=0;j<3;j++)
         {
             if(p[i][j]<range[j][0])
                 range[j][0]=p[i][j];
             if(p[i][j]>range[j][1])
                 range[j][1]=p[i][j];
         }

    }
}

void TriFace::calcN()
{
    a[0]=p[1][0]-p[0][0];
    a[1]=p[1][1]-p[0][1];
    a[2]=p[1][2]-p[0][2];

    b[0]=p[2][0]-p[0][0];
    b[1]=p[2][1]-p[0][1];
    b[2]=p[2][2]-p[0][2];

    n[0]=a[1]*b[2]-a[2]*b[1];
    n[1]=a[2]*b[0]-a[0]*b[2];
    n[2]=a[0]*b[1]-a[1]*b[0];
}

bool TriFace::isInZ(const float z)
{
    return ( (z>=range[2][0]) && (z<=range[2][1]));
}

bool TriFace::isInZ(const float zmin, const float zmax)
{
    if ( (zmin<=range[2][1]) && (zmax>=range[2][0]))
        return true;
    return false;
}


float TriFace::distanceFromArea(const float *x)
{
    float g[3]={x[0]-p[0][0],x[1]-p[0][1],x[2]-p[0][2]};
    return fabs(scalar(g,n));
}

bool TriFace::lineAtZ(const float z, /*line endpoints*/ float *aa, float *bb)
{
    /* line area intersection
     * (A+tL).n=P.n
     * n=(0,0,1)
     *Az+tLz=Pz
     *t=(Pz-Az)/Lz
     * Q=A+tL= A+L.(Pz-Az)/Lz

*/
    float t1,t2,t3;
    bool i1=true,i2=true,i3=true;
    if(a[2]!=0)
        t1=(z-p[0][2])/a[2];
    else
      i1=false;

    if(b[2]!=0)
        t2=(z-p[0][2])/b[2];
    else
      i2=false;

    if((a[2]-b[2])!=0)
        t3=(z-p[2][2])/(a[2]-b[2]);
    else
      i3=false;
    int cnt=0;
    if(i1)
    if(t1>0 && t1<1)
    {
        aa[0]=p[0][0]+t1*a[0];
        aa[1]=p[0][1]+t1*a[1];
        aa[2]=p[0][2]+t1*a[2];
        cnt=1;
    }
    if(i2)
    if(t2>0 && t2<1)
    {
        if(cnt==0)
        {
        aa[0]=p[0][0]+t2*b[0];
        aa[1]=p[0][1]+t2*b[1];
        aa[2]=p[0][2]+t2*b[2];
        }
        else
        {
        bb[0]=p[0][0]+t2*b[0];
        bb[1]=p[0][1]+t2*b[1];
        bb[2]=p[0][2]+t2*b[2];
        }
        cnt++;
    }
    if(i3)
    if(t3>0 && t3<1)
    {
        if(cnt==0)
        {
        aa[0]=p[2][0]+t3*(a[0]-b[0]);
        aa[1]=p[2][1]+t3*(a[1]-b[1]);
        aa[2]=p[2][2]+t3*(a[2]-b[2]);
        }
        else
            if(cnt==1)
        {
            bb[0]=p[2][0]+t3*(a[0]-b[0]);
            bb[1]=p[2][1]+t3*(a[1]-b[1]);
            bb[2]=p[2][2]+t3*(a[2]-b[2]);
        }
        else
            {
                cerr<<"all three intersect"<<endl;
                return false;
            }
        cnt++;
    }
    if(cnt==2)
        return true;
    else
        return false;
}
