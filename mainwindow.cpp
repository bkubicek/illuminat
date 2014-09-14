#include "mainwindow.h"

#include <iostream>

#include <QSlider>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPushButton>
#include <QFileDialog>
using namespace std;


#include "stlfile.h"
#include "layer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    bool full=false;
    loadSTL();

    gv=new QGraphicsView();
    gs=new QGraphicsScene();
    gv->setScene(gs);


    QImage im(100,100,QImage::Format_RGB888);
    im.fill(qRgb(128,128,128));
    im.setPixel(50,50,qRgb(255,128,128));
    pm=new QPixmap;
    pm->convertFromImage(im);
    gs->addPixmap(*pm);

    sl=new QSlider();

    sl->setRange(0,100);
    pbLoad=new QPushButton("load");
    QHBoxLayout *hb=new QHBoxLayout();
    if(!full)
      hb->addWidget(gv);
    hb->addWidget(sl);
    hb->addWidget(pbLoad);
    QWidget *foo=new QWidget;
    foo->setLayout(hb);
    setCentralWidget(foo);
    renderLayer(3);
    connect(sl,SIGNAL(valueChanged(int)),this, SLOT(slChanged(int)));
    connect(pbLoad,SIGNAL(clicked()),this, SLOT(pbLoadClicked()));
     gv->setStyleSheet( "QGraphicsView { border-style: none; }" );
     if(full)
    gv->showFullScreen();
}

MainWindow::~MainWindow()
{
    delete gv;
    exit(0);
}


 void MainWindow::loadSTL()
 {

     stl.read("test2.stl");
     //stl.read("D:\\me\\3d\\3dprints\\3D_Voronoi_Yoda_-_by_Dizingof.stl");
     cout<<"size:"<<stl.t.size()<<endl;
     stl.calcRange();

     int cnt=0;
     for(int i=0;i<stl.t.size();i++)
     {

         if(stl.t[i].isInZ(5.7,6))
             cnt++;

     }
     cout<<"in z:"<<cnt<<endl;
 }



void MainWindow::renderLayer(const float z)
{
    Layer l;
    l.sx=gv->width();
    l.sy=gv->height();

    l.fromStl(z,stl);



    pm->convertFromImage(l.toQImage());
    gs->addPixmap(*pm);
}

void MainWindow::slChanged(int i)
{
    renderLayer(stl.range[2][0]+(i-0.5)/float(100)*(stl.range[2][1]-stl.range[2][0]));

}

void MainWindow::pbLoadClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Object", "", "STL Files (*.stl *.STL *.Stl)");
    stl.read(fileName);
}
