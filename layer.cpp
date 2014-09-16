#include "layer.h"

#include <QImage>
#include <stack>
#include <iostream>
#include <list>
#include <QElapsedTimer>
using namespace  std;

Layer::Layer()
{
    sx=0;sy=0;
}

void Layer::bhm_line(int x1,int y1,int x2,int y2,char c)
{
 int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
 dx=x2-x1;
 dy=y2-y1;
 dx1=fabs(dx);
 dy1=fabs(dy);
 px=2*dy1-dx1;
 py=2*dx1-dy1;
 if(dy1<=dx1)
 {
  if(dx>=0)
  {
   x=x1;
   y=y1;
   xe=x2;
  }
  else
  {
   x=x2;
   y=y2;
   xe=x1;
  }
  bvec[x+sx*y]=c;
  for(i=0;x<xe;i++)
  {
   x=x+1;
   if(px<0)
   {
    px=px+2*dy1;
   }
   else
   {
    if((dx<0 && dy<0) || (dx>0 && dy>0))
    {
     y=y+1;
    }
    else
    {
     y=y-1;
    }
    px=px+2*(dy1-dx1);
   }
      bvec[x+sx*y]=c;
  }
 }
 else
 {
  if(dy>=0)
  {
   x=x1;
   y=y1;
   ye=y2;
  }
  else
  {
   x=x2;
   y=y2;
   ye=y1;
  }
     bvec[x+sx*y]=c;
  for(i=0;y<ye;i++)
  {
   y=y+1;
   if(py<=0)
   {
    py=py+2*dx1;
   }
   else
   {
    if((dx<0 && dy<0) || (dx>0 && dy>0))
    {
     x=x+1;
    }
    else
    {
     x=x-1;
    }
    py=py+2*(dx1-dy1);
   }
      bvec[x+sx*y]=c;
  }
 }
}

bool Layer::flootFill(char fillcol, char bcol)
{
    for(int y=1;y<sy;y++)
    {
        for(int x=1;x<sx;x++)
        {
             if(bvec[x+sx*y]==bcol)
                bvec[x+sx*y]=fillcol;
             else break;
        }

        for(int x=sx-2;x>0;x--)
        {
             if(bvec[x+sx*y]==bcol)
                bvec[x+sx*y]=fillcol;
             else break;
        }

    }


    bool anychange=false;
    bool change=false;
    int cnt=0;
   do
   {
        cnt++;
       change=false;
       for(int y=1;y<sy-1;y++)
       for(int x=1;x<sx-1;x++)
       {
           if(bvec[x+sx*y]!=bcol)
               continue;

           if(bvec[x-1+sx*y]==fillcol)
           {
               bvec[x+sx*y]=fillcol;
               change=true;
               continue;
           }
           if(bvec[x+1+sx*y]==fillcol)
           {
               bvec[x+sx*y]=fillcol;
               change=true;
               continue;
           }
          if(bvec[x+sx+sx*y]==fillcol)
           {
               bvec[x+sx*y]=fillcol;
               change=true;
               continue;
           }


           if(bvec[x-sx+sx*y]==fillcol)
           {
               bvec[x+sx*y]=fillcol;
               change=true;
               continue;
           }

       }
       if(change)
           anychange=true;

   }while(change && cnt<10000);
    return anychange;

}

void Layer::findAreasOld()
{
    //frame
    for(int x=0;x<sx;x++)
    {

        bvec[x+0]=fchar;
        bvec[x+sx*(sy-1)]=fchar;
    }
    for(int y=0;y<sy;y++)
    {

        bvec[0+sx*y]=fchar;
        bvec[sx-1+sx*y]=fchar;
    }

    flootFill( fchar, bchar);

    char newfill=fchar2;
      char oldfill=fchar;
      bool newfound=false;
      do
    {
    bool alternate=true;
    if(!alternate)
    {

        newfill=fchar;
        oldfill=fchar2;
    }
    else
    {
        newfill=fchar2;
        oldfill=fchar;

    }

    for(int y=1;y<sy-1;y++)
    for(int x=1;x<sx-1;x++)
    {
        if(bvec[x+sx*y]==lchar && bvec[x-1+sx*y]==oldfill && bvec[x+1+sx*y]==bchar)
        {

            bvec[x+1+sx*y]=newfill;newfound=true;
        }
        if(bvec[x+sx*y]==lchar && bvec[x+1+sx*y]==oldfill && bvec[x-1+sx*y]==bchar)
        {

            bvec[x-1+sx*y]=newfill;newfound=true;
        }

        if(bvec[x+sx*y]==lchar && bvec[x-sx+sx*y]==oldfill && bvec[x+sx+sx*y]==bchar)
        {

            bvec[x+sx+sx*y]=newfill;newfound=true;
        }
        if(bvec[x+sx*y]==lchar && bvec[x+sx+sx*y]==oldfill && bvec[x-sx+sx*y]==bchar)
        {

            bvec[x-sx+sx*y]=newfill;newfound=true;
        }

    }
    //bvec[sx/2+sx*(sy/2)]=fchar2;
    flootFill( newfill, bchar);
    alternate=!alternate;

    }while(newfound);


}

void Layer::findAreas()
{
    list<int> lpos;  //line positions that are in the source image
    for(int y=1;y<sy-1;y++)
    for(int x=1;x<sx-1;x++)
        if(bvec[sx*y+x]==lchar)
            lpos.push_back(sx*y+x);

    //bvec[0]=bchar;
    realFloodfill(fchar,bchar,0);
    bool newAreaFound;
    bool alternate=true;

    char newfill, oldfill;
    do
    {
        newAreaFound=false;

        if(!alternate)
        {
            newfill=fchar; oldfill=fchar2;
        }
        else
        {
            newfill=fchar2; oldfill=fchar;
        }

        //for(int i=0;i<lpos.size();i++)
         for(std::list<int>::iterator il = lpos.begin(), end = lpos.end(); il != end; ++il)
        {
            int p=*il;
            int newfound=-1;
            if( bvec[p-1]==oldfill && bvec[p+1]==bchar)
              newfound=p+1;
            else
            if( bvec[p+1]==oldfill && bvec[p-1]==bchar)
               newfound=p-1;
            else
            if( bvec[p+sx]==oldfill && bvec[p-sx]==bchar)
                newfound=p+sx;
            else
            if( bvec[p-sx]==oldfill && bvec[p+sx]==bchar)
                newfound=p-sx;
            if(newfound>0)
            {
                newAreaFound=true;
                //cerr<<"found new area:"<<newfound<<endl;
                realFloodfill(newfill,bchar,newfound);

            }
        }
        alternate=!alternate;

        //cerr<<"Starting to delete unnecessary line elements"<<lpos.size()<<endl;

        if(0)
        {
        std::list< int  >::iterator iter = lpos.begin();
        std::list< int  >::iterator end  = lpos.end();

        while (iter != lpos.end())
        {

            int p=*iter;
            if( !(   (bvec[p+1]==bchar)|| (bvec[p-1]==bchar) ||(bvec[p+sx]==bchar) || (bvec[p-sx]==bchar)))
                iter=lpos.erase(iter);
            else
                ++iter;


        }

        //cerr<<"now: "<<lpos.size()<<endl;
        }
    }while(newAreaFound);

}

void Layer::fromStl(float z, StlFile &stl)
{
    QElapsedTimer timer;
    timer.start();



    //const char bchar=0;




    bvec.resize(sx*sy,bchar);

    float xres=(stl.range[0][1]-stl.range[0][0])/float(sx-4);
    float yres=(stl.range[1][1]-stl.range[1][0])/float(sy-4);
    int band=stl.findBand(z);
    if(band>=0)
    for(int i=0;i<(int)stl.bands[band].size();i++)
    if(stl.t[stl.bands[band][i]].isInZ(z))
    {
        TriFace &t=stl.t[stl.bands[band][i]];

        float a[3],b[3];
        if(t.lineAtZ(z,a,b))
        {
            int ia[2],ib[2];
            ia[0]=int(2+(a[0]-stl.range[0][0])/xres);
            ia[1]=int(2+(a[1]-stl.range[1][0])/yres);
            ib[0]=int(2+(b[0]-stl.range[0][0])/xres);
            ib[1]=int(2+(b[1]-stl.range[1][0])/yres);

            bhm_line(ia[0],ia[1],ib[0],ib[1],lchar);


        }
    }
    cout<<"Time for lines:"<<timer.elapsed()<<endl;
    timer.start();


    //findAreasOld();
    findAreas();
    cout<<"Time for filling:"<<timer.elapsed()<<endl;


}

void Layer::realFloodfill(char fillcol, char bcol,int seedpos)
{
    //cout<<"floodfilling"<<endl;

    stack<int> s;
    /*for(int y=0;y<sy-1;y++)
    for(int x=0;x<sx-1;x++)
    if(bvec[x+sx*y]==fillcol)
    {
        s.push(x+sx*y);
    }
    */
    s.push(seedpos);
    //bvec[0]=bcol;
    while (!s.empty() )
    {
        int p= s.top();
        s.pop();
        if((p<0) || (p>sx*sy))
                continue;
    //cout<<"f1"<<endl;

        char val = bvec[p];
        if (val == bcol)
        {
            bvec[p] = fillcol;
            s.push(p+1);
            s.push(p+sx);
            s.push(p-1);

            s.push(p-sx);
        }
    }
}

QImage Layer::toQImage()
{
    //QElapsedTimer timer;    timer.start();

    QImage im(sx,sy,QImage::Format_ARGB32);
    //cout<<"Im creation:"<<timer.elapsed()<<endl;timer.start();


     const QRgb fc=qRgba(255,0,0,255);
    const QRgb fc2=qRgba(255,255,0,255);

    const QRgb lc=qRgba(128,128,128,255);

    const QRgb bc=qRgba(0,0,0,255);
    im.fill(bc);
    //cout<<"Im fill:"<<timer.elapsed()<<endl;timer.start();

    int p=0;
    for( int i = 0; i < sy; ++i)
    {
        QRgb *scL = reinterpret_cast< QRgb *>( im.scanLine( i ) );

        for( int j = 0; j < sx; ++j)
        {
            switch(bvec[p++])
            {
            //case fchar : scL[j]=fc ;break;
            case fchar2: scL[j]=fc2;break;
            case bchar : scL[j]=bc ;break;
            case lchar:  scL[j]=lc;break;
           // case fchar2: scL[j]=qRgba(255,255,255,255);break
            //case lchar:  scL[j]=qRgba(0,255,0,255);break;
            }
        }
    }
    //cout<<"Im color:"<<timer.elapsed()<<endl;timer.start();
    return im;
}
