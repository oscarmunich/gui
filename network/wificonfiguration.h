#ifndef WIFICONFIGURATION_H
#define WIFICONFIGURATION_H

#include "configuration.h"

#include <QQmlListProperty>

//this class represents Wireless Access Point
class AccessPoint : QObject {
    Q_OBJECT

    Q_PROPERTY(QString ssid READ ssid)
    Q_PROPERTY(int stregth READ stregth)
    Q_PROPERTY(bool isPasswordRequired READ isPasswordRequired)

public:
    AccessPoint(const QString &ssid, int stregth, bool isPasswordRequired, QObject *parent = 0);
    ~AccessPoint();

    QString ssid() const;
    int stregth() const;
    bool isPasswordRequired() const;

private:
    QString m_ssid;
    int m_stregth;
    bool m_isRequiresPassword;
};

class WiFiConfiguration : public Configuration
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<AccessPoint> accessPoints READ accessPoints NOTIFY accessPointsChanged)

public:
    WiFiConfiguration(QObject *parent = 0);
    ~WiFiConfiguration();

    QQmlListProperty<AccessPoint> accessPoints();

public slots:
    void requestAccessPoints();
    void connectAccessPoint(const QString &ssid, const QString &password = QString());
    void disconnectAccessPoint();

    void downInterface();

signals:
    void accessPointsChanged();
    void wapConnected(const QString &ssid);
    void wapDisconnected(const QString &ssid);

private:
    void handleResponse(const QString &command, const QString &response);
    void clearAccessPointsList();

    void updateAccessPoints(const QString &response);
    void updateWapConnection(const QString &command, const QString &response, bool connect);

    QList<AccessPoint *> m_accessPoints;
    QString m_connectedSsid;
};

#endif // WIFICONFIGURATION_H
