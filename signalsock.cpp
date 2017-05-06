#include <QtNetwork>
#include "signalsock.h"
#include "Backend.h"

signalSock::signalSock(QObject *parent) : QObject(parent)
{
    initSocket();
}

void signalSock::initSocket()
{
    qDebug() << "initSocket" << 9999;
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::LocalHost, 9999);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void signalSock::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        qDebug() << "readPendingDatagrams" << datagram << time(0);
        if (datagram.toLower() == "p off") {
            gBackend->setPower(false);
            continue;
        }
        if (datagram.toLower() == "p on") {
            gBackend->setPower(true);
            continue;
        }
    }
}
