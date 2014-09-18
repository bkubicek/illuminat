#include "illuminator.h"

#include "layer.h"
#include "motion.h"
#include "settings.h"

#include <QTimer>
#include <iostream>

using namespace std;

Illuminator::Illuminator(QObject *parent) :
    QGraphicsView()
{

    gs=new QGraphicsScene();
    setScene(gs);
    setStyleSheet( "QGraphicsView { border-style: none; }" );
    set=new Settings();
    mo=new Motion(set);
    stl=new StlFile(set);

    pm=new QPixmap();
    imNextLayer=new QImage();
    renderBlack();

    cmd=new WebCommandInterface(this, mo,set) ;
}

void Illuminator::prepareNextLayer(const float z)
{
    Layer l;
    l.sx=width();
    l.sy=height();
    l.fromStl(z,*stl);

    *imNextLayer=l.toQImage();
}

void Illuminator::displayPreparedLayer()
{


    pm->convertFromImage(*imNextLayer);
    gs->addPixmap(*pm);
}

void Illuminator::renderBlack()
{

    QImage im(width(),height(),QImage::Format_RGB888);



    const QRgb bc=qRgb(0,0,0);

    im.fill(bc);



    pm->convertFromImage(im);
    gs->addPixmap(*pm);
}

void Illuminator::runClicked()
{
    set->currentz=stl->range[2][0];
    set->printing=true;

    mo->doStart();
    perform();
}

void Illuminator::perform()
{
    cout.flush();
    cerr<<"Starting perform"<<endl;
    renderBlack();
    mo->doLayerChange();
    prepareNextLayer(set->currentz);
    set->currentz+=set->layerheight;

    if(set->currentz<stl->range[2][1])
    {
        displayPreparedLayer();
        QTimer::singleShot(set->exposure*1000, this, SLOT(perform()));
    }
    else
    {
         cerr<<"outside"<<endl;
        renderBlack();
        mo->doEnd();
        set->printing=false;
        //cerr<<"exiting now"<<endl;exit(0);
    }
}

void Illuminator::loadSTL()
{

    stl->read("C:/Users/BK/Documents/illuminat/test.stl");
    //stl.read("D:\\me\\3d\\3dprints\\3D_Voronoi_Yoda_-_by_Dizingof.stl");
    cout<<"size:"<<stl->t.size()<<endl;
    stl->calcRange();

    int cnt=0;
    for(int i=0;i<int(stl->t.size());i++)
    {

        if(stl->t[i].isInZ(5.7,6))
            cnt++;

    }
    cout<<"in z:"<<cnt<<endl;
}

