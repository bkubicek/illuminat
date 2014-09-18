#ifndef WEBCOMMANDINTERFACE_H
#define WEBCOMMANDINTERFACE_H

#include <QObject>
#include <QTextStream>

#include "motion.h"
#include "illuminator.h"

class QTcpServer;
class Settings;
class Illuminator;

class WebCommandInterface : public QObject
{
    Q_OBJECT
public:
    explicit WebCommandInterface(Illuminator *_ill,Motion *_motion, Settings *_set, QObject *parent = 0);


    void htmlStart(QTextStream &os);
    void html404(QTextStream &os);
    void htmlCurSlice(QTextStream &os);

    Settings *set;
    Illuminator *ill;

signals:

public slots:

void newConnection();
void discardClient();
void readClient();

private:
    QTcpServer *server;
    Motion *motion;
};

#endif // WEBCOMMANDINTERFACE_H
