#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stlfile.h"


class QSlider;

class QPushButton;
class Motion;
class Illuminator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();




    Illuminator *il;

     QSlider *sl;
     QPushButton *pbLoad;
     QPushButton *pbRun;


public slots:
     void slChanged(int);
     void pbLoadClicked();


};

#endif // MAINWINDOW_H
