#include "webcommandinterface.h"

#include <QTcpSocket>
#include <QTcpServer>
#include <QBuffer>
#include <iostream>
#include "settings.h"
using namespace std;

WebCommandInterface::WebCommandInterface(Illuminator *_ill,Motion *_motion, Settings *_set, QObject *parent) :
    QObject(parent)
{
    motion=_motion;
    set=_set;
    ill=_ill;
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

void WebCommandInterface::htmlStart(QTextStream &os)
{
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n"
          ;
    os<<"<head><meta http-equiv=\"refresh\" content=\"5\"></head>";
    os<<"<h1>Illuminator State:</h1>\n";

    os<<"<table><tr><th>Variable:</th><th>Value</th></tr>\n";
    os<<"<tr><td>STL loaded:</td><td>"<<((set->stlLoaded)?"Yes":"No")<<"</td></tr>\n";
    os<<"<tr><td>STL filename:</td><td>"<<set->stlName<<"</td></tr>\n";
    os<<"<tr><td>Building:</td><td>"<<((set->printing)?"Yes":"No")<<"</td></tr>\n";
    os<<"<tr><td>Currentz:</td><td>"<<set->currentz<<"</td></tr>\n";

}

void WebCommandInterface::html404(QTextStream &os)
{
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n"
          ;
    os<<"<h1>Page does not exist</h1>\n";
    //os<<"<h1>Nothing to see here</h1>\n";
}

void WebCommandInterface::htmlCurSlice(QTextStream &os)
{
      QByteArray ba;
      QBuffer buffer(&ba);
      buffer.open(QIODevice::WriteOnly);
      ill->imNextLayer->save(&buffer, "PNG"); // writes image into ba in PNG format
    os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: image/png; charset=\"utf-8\"\r\n"
        "\r\n"
          ;
    os.write(buffer.data(),buffer.size());
    os<<buffer;
    //os<<"<h1>Nothing to see here</h1>\n";
}

void WebCommandInterface::readClient()
{
    //if (disabled)
    //    return;
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (!socket->canReadLine() )
        return;

    QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
    for(int i=0;i<tokens.size();i++)
        cout<<"Got: "<<tokens[i].toStdString()<<endl;
    if (tokens[0] == "GET")
    {
        QTextStream os(socket);
        os.setAutoDetectUnicode(true);

        if(tokens[1]=="/")
        {
            htmlStart(os);
        }
        if(tokens[1]=="/cur.png")
        {
            htmlCurSlice(os);
        }
        else
            html404(os);

        socket->close();

        cout<<"Srv: wrote to client"<<endl;


        if (socket->state() == QTcpSocket::UnconnectedState) {
            delete socket;

            cout<<"Srv: Connection closed"<<endl;
        }
    }
}
void WebCommandInterface::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    cout<<"Srv: Connection closed"<<endl;
}
