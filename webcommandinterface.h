#ifndef WEBCOMMANDINTERFACE_H
#define WEBCOMMANDINTERFACE_H

#include <QObject>

#include "motion.h"

class QTcpServer;

class WebCommandInterface : public QObject
{
    Q_OBJECT
public:
    explicit WebCommandInterface(Motion *_motion, QObject *parent = 0);

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
