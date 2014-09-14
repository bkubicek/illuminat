#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stlfile.h"

class QPixmap;
class QGraphicsScene;
class QSlider;
class QGraphicsView ;
class QPushButton;
class Motion;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loadSTL();
     StlFile stl;


     void renderLayer(const float z);
     void renderBlack();
     QPixmap *pm;
     QGraphicsScene *gs;
     QGraphicsView *gv;
     QSlider *sl;
     QPushButton *pbLoad;
     QPushButton *pbRun;

     float currentz;
     Motion *mo;
public slots:
     void slChanged(int);
     void pbLoadClicked();

     void runClicked();
     void perform();
};

#endif // MAINWINDOW_H
