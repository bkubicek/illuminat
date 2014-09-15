#include "illuminator.h"

#include "layer.h"
#include "motion.h"
#include <QTimer>
#include <iostream>

using namespace std;

Illuminator::Illuminator(QObject *parent) :
    QGraphicsView()
{
    gs=new QGraphicsScene();
    setScene(gs);
    setStyleSheet( "QGraphicsView { border-style: none; }" );
    mo=new Motion();

    pm=new QPixmap();
    imNextLayer=new QImage();
    renderBlack();
}

void Illuminator::prepareNextLayer(const float z)
{
    Layer l;
    l.sx=width();
    l.sy=height();
    l.fromStl(z,stl);

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
    currentz=stl.range[2][0];
    perform();
    mo->doStart();
}

void Illuminator::perform()
{
    renderBlack();
    mo->doLayerChange();
    prepareNextLayer(currentz);
    currentz+=mo->layerheight;
    if(currentz<stl.range[2][1])
    {
        displayPreparedLayer();
     QTimer::singleShot(mo->exposure*1000, this, SLOT(perform()));
    }
    else
    {
        renderBlack();
        mo->doEnd();
	exit(0);
    }
}

void Illuminator::loadSTL()
{

    stl.read("C:/Users/BK/Documents/illuminat/test.stl");
    //stl.read("D:\\me\\3d\\3dprints\\3D_Voronoi_Yoda_-_by_Dizingof.stl");
    cout<<"size:"<<stl.t.size()<<endl;
    stl.calcRange();

    int cnt=0;
    for(int i=0;i<int(stl.t.size());i++)
    {

        if(stl.t[i].isInZ(5.7,6))
            cnt++;

    }
    cout<<"in z:"<<cnt<<endl;
}

/*
void Illuminator::resized()
{
    renderBlack();
}
*/
