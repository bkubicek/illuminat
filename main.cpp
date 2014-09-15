#include "mainwindow.h"
#include "illuminator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    if(argc==1)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
    }
    else //decicated mode
    {
        QApplication a(argc, argv);
        Illuminator il;
        il.showFullScreen();
        il.show();
        il.stl.read((argv[1]));;
        il.runClicked();
        return a.exec();

    }
}
