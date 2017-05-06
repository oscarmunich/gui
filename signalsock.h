#ifndef SIGNALSOCK_H
#define SIGNALSOCK_H

#include <QtGui>
#include <QUdpSocket>

class signalSock : public QObject
{
    Q_OBJECT
public:
    signalSock(QObject *parent = 0);
    void initSocket();
public slots:
    void readPendingDatagrams();
private:
    QUdpSocket *udpSocket;
};

#endif // SIGNALSOCK_H
