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

    //socket->write("Hello client\r\n");
   /* socket->write("<html><body>html</body></html>\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
    */
    connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(discardClient()));

}

void WebCommandInterface::readClient()
{
    //if (disabled)
    //    return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        for(int i=0;i<tokens.size();i++)
            cout<<"Got: "<<tokens[i].toStdString()<<endl;
        if (tokens[0] == "GET") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                "Content-Type: text/html; charset=\"utf-8\"\r\n"
                "\r\n"
                "<h1>Nothing to see here</h1>\n"
               <<"footbar" << "\n";
            socket->close();

            cout<<"Srv: wrote to client"<<endl;
            //QtServiceBase::instance()->logMessage("Wrote to client");

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                //QtServiceBase::instance()->logMessage("Connection closed");
                cout<<"Srv: Connection closed"<<endl;
            }

        }
    }
}
void WebCommandInterface::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    cout<<"Srv: Connection closed"<<endl;
}
