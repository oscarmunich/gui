#include "monitorserver.h"
#include "Backend.h"

#include <QTcpSocket>
#include <QRegExp>

MonitorServer::MonitorServer(QObject *parent) : QTcpServer(parent)
{
}

void MonitorServer::incomingConnection(qintptr socketfd)
{
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketfd);
    clients.insert(client);

    qDebug() << "New client from:" << client->peerAddress().toString();

    connect(client, SIGNAL(readyRead()),    this, SLOT(readyRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    client->write("CONNECTED\n");
}

void MonitorServer::readyRead()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    while(client->canReadLine()) {
        QString line = QString::fromUtf8(client->readLine()).trimmed();
        qDebug() << "Line in:" << line;
        if (!line.startsWith("I ")) {
            qDebug() << "ignored:" << line;
            continue;
        }
        qDebug() << "Read line:" << line;
        QStringList ql = line.split(' ');

        int index = ql[1].toInt();
        ql.pop_front();
        ql.pop_front();
        QString value = ql.join(" ");
        switch (index) {
        case 9:  gBackend->setDisk(value); break;
        case 10:  gBackend->setFile(value); break;
        case 11:  gBackend->setBlock(value); break;
        case 12:  gBackend->setBlocks(value); break;
        case 17:  gBackend->setStatus(value); break;
        case 18:  gBackend->setRemainingTime(value); break;
        case 19:  gBackend->setUpdateProgress(value); break;
        case 20:  gBackend->setDisks(value); break;
        case 21:  gBackend->setElapsedTime(value); break;
        case 22:  gBackend->addLog(value); break;
        }
    }
}

void MonitorServer::disconnected()
{
    QTcpSocket *client = (QTcpSocket*)sender();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();

    clients.remove(client);

    gBackend->dataLoadStopped();

    QString user = users[client];
    users.remove(client);

    sendUserList();
    foreach(QTcpSocket *client, clients)
        client->write(QString("Server:" + user + " has left.\n").toUtf8());
}

void MonitorServer::send2Clients(QString message)
{
    foreach(QTcpSocket *client, clients)
        client->write(message.toUtf8());
}

void MonitorServer::sendUserList()
{
    QStringList userList;
    foreach(QString user, users.values())
        userList << user;

    foreach(QTcpSocket *client, clients)
        client->write(QString("/users:" + userList.join(",") + "\n").toUtf8());
}
