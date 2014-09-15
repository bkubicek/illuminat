#include "layer.h"

#include <QImage>
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

void Layer::fromStl(float z, StlFile &stl)
{


    //const char bchar=0;




    bvec.resize(sx*sy,bchar);

    float xres=(stl.range[0][1]-stl.range[0][0])/float(sx-4);
    float yres=(stl.range[1][1]-stl.range[1][0])/float(sy-4);
    int band=stl.findBand(z);
    if(band>=0)
    for(int i=0;i<stl.bands[band].size();i++)
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
            /*
            float l=2*sqrt(pow(ia[0]-ib[0],2)+pow(ia[1]-ib[1],2));
            for(float t=0;t<=1;t+=1/l)
            {
                float x,y;
                //x=ia[0]+t*(ib[0]-ia[0]);
                //y=ia[1]+t*(ib[1]-ia[1]);
                //im.setPixel(2+(x-stl.range[0][0])/xres,2+(y-stl.range[1][0])/yres,qRgb(128,128,128));
                //int xx=int(2+(x-stl.range[0][0])/xres);
                //int yy=int(2+(y-stl.range[1][0])/yres);
                int xx=ia[0]+t*(ib[0]-ia[0]);;
                int yy=ia[1]+t*(ib[1]-ia[1]);
                if((xx>0) && (xx <sx) &&(yy>0) && (yy<sy))
                bvec[xx+sx*yy]=lchar;
            }
            */

            bhm_line(ia[0],ia[1],ib[0],ib[1],lchar);


        }
    }
    //im.setPixel(50,50,qRgb(255,128,128));
    if(1 /*do filling */)
    {
    for(int x=0;x<sx;x++)
    {
        //im.setPixel(x,0,fc);
        //im.setPixel(x,sy-1,fc);
        bvec[x+0]=fchar;
        bvec[x+sx*(sy-1)]=fchar;
    }
    for(int y=0;y<sy;y++)
    {
        //im.setPixel(0,y,fc);
        //im.setPixel(sx-1,y,fc);
        bvec[0+sx*y]=fchar;
        bvec[sx-1+sx*y]=fchar;
    }




    flootFill( fchar, bchar);

    bool alternate=true;

    char newfill=fchar2;
    char oldfill=fchar;
    bool newfound=false;
    do
    {
        newfound=false;
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
    } //dofilling
}

QImage Layer::toQImage()
{
    QImage im(sx,sy,QImage::Format_RGB888);


    const QRgb fc=qRgb(255,0,0);
    const QRgb fc2=qRgb(255,255,0);
    const QRgb bc=qRgb(0,0,0);
    const QRgb lc=qRgb(128,128,128);
    im.fill(bc);
    for(int y=0;y<sy;y++)
    for(int x=0;x<sx;x++)
    {
        /*
        if(bvec[x+sx*y]==fchar)
            im.setPixel(x,y,fc);
        if(bvec[x+sx*y]==fchar2)
            im.setPixel(x,y,fc2);
        if(bvec[x+sx*y]==bchar)
            im.setPixel(x,y,bc);
        if(bvec[x+sx*y]==lchar)
            im.setPixel(x,y,lc);
            */
        if(bvec[x+sx*y]==fchar2)
            im.setPixel(x,y,qRgb(255,255,255));
        if(bvec[x+sx*y]==lchar)
            im.setPixel(x,y,qRgb(255,0,0));

    }
    return im;
}
