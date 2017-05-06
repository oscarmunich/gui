#include "wificonfiguration.h"
#include <QDebug>
AccessPoint::AccessPoint(const QString &ssid, int stregth, bool isRequiresPassword, QObject *parent)
    : QObject(parent)
    , m_ssid(ssid)
    , m_stregth(stregth)
    , m_isRequiresPassword(isRequiresPassword)
{
}

AccessPoint::~AccessPoint()
{
}

QString AccessPoint::ssid() const
{
    return m_ssid;
}

int AccessPoint::stregth() const
{
    return m_stregth;
}

bool AccessPoint::isPasswordRequired() const
{
    return m_isRequiresPassword;
}

WiFiConfiguration::WiFiConfiguration(QObject *parent)
    : Configuration("wi-fi", parent)
{
}

WiFiConfiguration::~WiFiConfiguration()
{
    clearAccessPointsList();
}

QQmlListProperty<AccessPoint> WiFiConfiguration::accessPoints()
{
    return QQmlListProperty<AccessPoint>(this, m_accessPoints);
}

void WiFiConfiguration::requestAccessPoints()
{
    exec("C GetWiFiAccessPoints");
}

void WiFiConfiguration::connectAccessPoint(const QString &ssid, const QString &password)
{
        QString commandString = QString("C ConnectWAP %1|%2").arg(ssid).arg(password);
        m_values.insert("ConnectWAP", ssid);
        exec(commandString);
}

void WiFiConfiguration::disconnectAccessPoint()
{
        QString commandString = QString("C DisconnectWAP %1").arg(m_connectedSsid);
        exec(commandString);
}

void WiFiConfiguration::downInterface()
{
    clearAccessPointsList();
    emit accessPointsChanged();

    Configuration::downInterface();
}


void WiFiConfiguration::clearAccessPointsList()
{
    qDeleteAll(m_accessPoints);
    m_accessPoints.clear();
}

void WiFiConfiguration::updateAccessPoints(const QString &response)
{
    int oldCount = m_accessPoints.count();
    clearAccessPointsList();

    if (!response.isEmpty()) {
        QStringList accessPoints = response.split("|");
        //instantiate received access points
        foreach(QString accessPoint, accessPoints) {
            accessPoint.remove("<");
            accessPoint.remove(">");

            QStringList values = accessPoint.split("#");
            if (values.size() == 3) {
                QString ssid = values.at(0);
                int stregth = values.at(1).toInt();
                bool isPasswordRequired = values.at(1) == "true";
                m_accessPoints.append(new AccessPoint(ssid, stregth, isPasswordRequired));
            }
        }
    }

    if (oldCount != m_accessPoints.count())
        emit accessPointsChanged();
}

void WiFiConfiguration::updateWapConnection(const QString &command, const QString &response, bool connect)
{
    if (response == "OK") {
        if (connect) {
            m_connectedSsid = m_values[command].toString();
            emit wapConnected(m_connectedSsid);
        } else {
            emit wapDisconnected(m_connectedSsid);
            m_connectedSsid.clear();
        }
    }
}


void WiFiConfiguration::handleResponse(const QString &command, const QString &response)
{
    if (command == "GetWiFiAccessPoints") {
        updateAccessPoints(response);
        return;
    } else if (command == "ConnectWAP") {
        updateWapConnection(command, response, true);
        return;
    } else if (command == "DisconnectWAP") {
        updateWapConnection(command, response, true);
        return;
    }

    Configuration::handleResponse(command, response);
}
