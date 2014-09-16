#include "webcommandinterface.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <iostream>
using namespace std;

WebCommandInterface::WebCommandInterface(Motion *_motion, QObject *parent) :
    QObject(parent)
{
    motion=_motion;
    server = new QTcpServer(this);

        // whenever a user connects, it will emit signal
        connect(server, SIGNAL(newConnection()),
                this, SLOT(newConnection()));

        if(!server->listen(QHostAddress::Any, 9999))
        {
            cout << "Server could not start";
        }
        else
        {
            cout<< "Server started!";
        }
}


void WebCommandInterface::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();

    socket->write("Hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}
