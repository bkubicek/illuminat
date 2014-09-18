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

#include "motion.h"
#include "illuminator.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    bool full=false;


    il=new Illuminator();
    //gv=new QGraphicsView();
    il->loadSTL();




    sl=new QSlider();

    sl->setRange(0,100);
    pbLoad=new QPushButton("load");
    pbRun=new QPushButton("run");
    QHBoxLayout *hb=new QHBoxLayout();
    if(!full)
      hb->addWidget(il);
    hb->addWidget(sl);
    hb->addWidget(pbLoad);
    hb->addWidget(pbRun);
    QWidget *foo=new QWidget;
    foo->setLayout(hb);
    setCentralWidget(foo);

    connect(sl,SIGNAL(valueChanged(int)),this, SLOT(slChanged(int)));
    connect(pbLoad,SIGNAL(clicked()),this, SLOT(pbLoadClicked()));
    connect(pbRun,SIGNAL(clicked()),il, SLOT(runClicked()));
    //connect(this,SIGNAL(resizeEvent()),il, SLOT(runClicked()));

     if(full)
    il->showFullScreen();
     showMaximized();


    il->renderBlack();
}

MainWindow::~MainWindow()
{
   // delete gv;
    exit(0);
}


void MainWindow::slChanged(int i)
{
    il->renderBlack();
    il->set->currentz=il->stl->range[2][0]+(i-0.5)/float(100)*(il->stl->range[2][1]-il->stl->range[2][0]);
    il->prepareNextLayer(il->set->currentz);

    il->displayPreparedLayer();
}

void MainWindow::pbLoadClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Object", "", "STL Files (*.stl *.STL *.Stl)");
    il->stl->read(fileName);
}

