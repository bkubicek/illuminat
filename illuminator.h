#ifndef ILLUMINATOR_H
#define ILLUMINATOR_H

#include <QGraphicsView>


class QPixmap;
class QGraphicsScene;
class QGraphicsView ;
class Motion;
class QImage;

#include "stlfile.h"

/* The widget that displays the projector content */
class Illuminator : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Illuminator(QObject *parent = 0);

    void displayPreparedLayer();
    void prepareNextLayer(const float z);
    void renderBlack();
    void loadSTL();

    StlFile stl;
    float currentz;
    Motion *mo;
    QImage *imNextLayer;

signals:

public slots:

    void runClicked();
    void perform();


private:
    QPixmap *pm;
    QGraphicsScene *gs;

};




#endif // ILLUMINATOR_H
