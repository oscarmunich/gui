#include "confighelper.h"

#include <QUdpSocket>
#include <QDebug>
#include <QString>

ConfigHelper* ConfigHelper::m_instance = 0;
int ConfigHelper::m_portdelta = 0;

ConfigHelper::ConfigHelper(QObject *parent) :
    QObject(parent)
  , m_commandSocket(new QUdpSocket(this))
{
    mConnected = false;
}

void ConfigHelper::connect2server() {
    if (mConnected) {
        return;
    }
    qDebug()<<"ConfigHelper::connect";
    m_commandSocket->bind(QHostAddress::LocalHost, 4000 + m_portdelta);

    connect(m_commandSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    mConnected = true;
}

ConfigHelper::~ConfigHelper()
{
}

ConfigHelper::ConfigHelper(const ConfigHelper &)
{
}

ConfigHelper &ConfigHelper::operator =(const ConfigHelper &)
{
    return *this;
}


ConfigHelper *ConfigHelper::instance()
{
    if (!m_instance)
        m_instance = new ConfigHelper;

    return m_instance;
}

void ConfigHelper::execute(const QString &command)
{
    connect2server();
    qDebug()<<"Client's request command: "<<command;
//    if (!command.isEmpty())
//        m_commandSocket->writeDatagram(QString::fromLatin1(command.data()), QHostAddress::LocalHost, 6000 + m_portdelta);
}


void ConfigHelper::onReadyRead()
{
    QByteArray datagram;
    QHostAddress address;
    quint16 port;

    //qDebug() << "ConfigHelper::onReadyRead";
    while (m_commandSocket->hasPendingDatagrams()) {
        datagram.resize(m_commandSocket->pendingDatagramSize());
        m_commandSocket->readDatagram(datagram.data(), datagram.size(), &address, &port);
        qDebug()<<"Config response: "<<datagram;
        int firstSpace = 1;
        int secondSpace = datagram.indexOf(" ", 3) + 1;
        QString command = datagram.mid(firstSpace + 1, secondSpace - 3);
        QString response = secondSpace ? datagram.mid(secondSpace, datagram.size() - secondSpace) : QString();

        emit reply(command, response);
    }
}
