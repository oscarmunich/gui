#ifndef MONITORSERVER_H
#define MONITORSERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QSet>

class MonitorServer : public QTcpServer
{
    Q_OBJECT

    public:
        MonitorServer(QObject *parent=0);
        void send2Clients(QString message);

    private slots:
        void readyRead();
        void disconnected();
        void sendUserList();

    protected:
        virtual void incomingConnection(qintptr socketfd);

    private:
        QSet<QTcpSocket*> clients;
        QMap<QTcpSocket*,QString> users;
};

#endif // MONITORSERVER_H
