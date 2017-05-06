#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>

class QUdpSocket;

class ConfigHelper : public QObject
{
    Q_OBJECT
public:
    static ConfigHelper* instance();
    static int m_portdelta;

    Q_INVOKABLE void execute(const QString &command);

signals:
    void reply(const QString &command, const QString &response);

private slots:
    void onReadyRead();

private:
    bool mConnected;
    void connect2server();

    explicit ConfigHelper(QObject *parent = 0);
    ~ConfigHelper();
    ConfigHelper(const ConfigHelper &);
    ConfigHelper& operator=(const ConfigHelper &);

    static ConfigHelper* m_instance;

    QUdpSocket *m_commandSocket;
};

#endif // CONFIGHELPER_H
