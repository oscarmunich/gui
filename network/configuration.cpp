#include "configuration.h"

#include "helpers/confighelper.h"
#include <QEventLoop>
#include <QTimer>
#include <QVariant>

Configuration::Configuration(const QString &configName, QObject *parent) :
    QObject(parent)
  , m_active(false)
  , m_isUseDhcp(false)
  , m_configName(configName)
  , m_configHelper(ConfigHelper::instance())
{
    connect(m_configHelper, SIGNAL(reply(QString,QString)), this, SLOT(handleResponse(QString,QString)));

    //get interface name
    exec(QString("C GetInterface %1").arg(m_configName));
}

Configuration::~Configuration()
{
}

bool Configuration::active() const
{
    return m_active;
}

void Configuration::setUseDhcp(bool use)
{
    if (m_isUseDhcp != use) {
        QString commandString = QString("C SetDhcpEnabled %1|%2").arg(m_interfaceName).arg(use);
        m_values.insert("SetDhcpEnabled", use);
        exec(commandString);
    }
}

bool Configuration::isUseDhcp() const
{
    return m_isUseDhcp;
}

void Configuration::setIpAddress(const QString &ip)
{
    if (m_ipAddress != ip) {
        QString commandString = QString("C SetIpAddress %1|%2").arg(m_interfaceName).arg(ip);
        m_values.insert("SetIpAddress", ip);
        exec(commandString);
    }
}

QString Configuration::ipAddress() const
{
    return m_ipAddress;
}

void Configuration::setSubnetMask(const QString &mask)
{
    if (m_subnetMask != mask) {
        QString commandString = QString("C SetSubnetMask %1|%2").arg(m_interfaceName).arg(mask);
        m_values.insert("SetSubnetMask", mask);
        exec(commandString);
    }
}

QString Configuration::subnetMask() const
{
    return m_subnetMask;
}

void Configuration::setDefaultGateway(const QString &gateway)
{
    if (m_defaultGateway != gateway) {
        QString commandString = QString("C SetDefaultGateway %1|%2").arg(m_interfaceName).arg(gateway);
        m_values.insert("SetDefaultGateway", gateway);
        exec(commandString);
    }
}

QString Configuration::defaultGateway() const
{
    return m_defaultGateway;
}

void Configuration::setPrimaryDns(const QString &dns)
{
    if (m_primaryDns != dns) {
        QString commandString = QString("C SetPrimaryDNS %1|%2").arg(m_interfaceName).arg(dns);
        m_values.insert("SetPrimaryDNS", dns);
        exec(commandString);
    }
}

QString Configuration::primaryDns() const
{
    return m_primaryDns;
}

void Configuration::setSecondaryDns(const QString &dns)
{
    if (m_secondaryDns != dns) {
        QString commandString = QString("C SetSecondaryDNS %1|%2").arg(m_interfaceName).arg(dns);
        m_values.insert("SetSecondaryDNS", dns);
        exec(commandString);
    }
}

QString Configuration::secondaryDns() const
{
    return m_secondaryDns;
}

void Configuration::upInterface()
{
    //if the interface is down
    if (!m_active) {
        QString commandString = QString("C SetInterfaceEnabled %1|%2").arg(m_interfaceName).arg("true");
        m_values.insert("SetInterfaceEnabled", true);
        exec(commandString);
    }
}

void Configuration::downInterface()
{
    //if the interface is up
    if (m_active) {
        QString commandString = QString("C SetInterfaceEnabled %1|%2").arg(m_interfaceName).arg("false");
        m_values.insert("SetInterfaceEnabled", false);
        exec(commandString);
    }
}


void Configuration::exec(const QString &command)
{
    m_configHelper->execute(command);
}

void Configuration::handleResponse(const QString &command, const QString &response)
{
    if (command == "GetInterface") {
        updateInterface(response);
        return;
    } else if (command == "SetDhcpEnabled") {
        updateDhcpEnabled(command, response);
        return;
    } else if (command == "SetIpAddress") {
        updateIpAddress(command, response);
        return;
    }  else if (command == "SetSubnetMask") {
        updateSubnetMask(command, response);
        return;
    } else if (command == "SetDefaultGateway") {
        updateDefaultGateway(command, response);
        return;
    } else if (command == "SetPrimaryDNS") {
        updatePrimaryDns(command, response);
        return;
    } else if (command == "SetSecondaryDNS") {
        updateSecondaryDns(command, response);
        return;
    } else if (command == "SetInterfaceEnabled") {
        updateInterfaceActivity(command, response);
        return;
    }
}

void Configuration::updateInterface(const QString &response)
{
    m_interfaceName = response.mid(1, response.size() - 2);
}

void Configuration::updateDhcpEnabled(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_isUseDhcp = m_values[command].toBool();
        emit isUseDhcpChanged(m_isUseDhcp);
    }
}

void Configuration::updateIpAddress(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_ipAddress = m_values[command].toString();
        emit ipAddressChanged(m_ipAddress);
    }
}

void Configuration::updateSubnetMask(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_subnetMask = m_values[command].toString();
        emit subnetMaskChanged(m_subnetMask);
    }
}

void Configuration::updateDefaultGateway(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_defaultGateway = m_values[command].toString();
        emit defaultGatewayChanged(m_defaultGateway);
    }
}

void Configuration::updatePrimaryDns(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_primaryDns = m_values[command].toString();
        emit primaryDnsChanged(m_primaryDns);
    }
}

void Configuration::updateSecondaryDns(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_secondaryDns = m_values[command].toString();
        emit secondaryDnsChanged(m_secondaryDns);
    }
}

void Configuration::updateInterfaceActivity(const QString &command, const QString &response)
{
    if (response == "OK") {
        m_active = m_values[command].toBool();
        emit activeChanged();
    }
}
