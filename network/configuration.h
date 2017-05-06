#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QStringList>
#include <QHash>

class ConfigHelper;

// This class represents common network configuration
class Configuration : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active NOTIFY activeChanged)
    Q_PROPERTY(bool useDhcp READ isUseDhcp WRITE setUseDhcp NOTIFY isUseDhcpChanged)
    Q_PROPERTY(QString ipAddress READ ipAddress WRITE setIpAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(QString subnetMask READ subnetMask WRITE setSubnetMask NOTIFY subnetMaskChanged)
    Q_PROPERTY(QString defaultGateway READ defaultGateway WRITE setDefaultGateway NOTIFY defaultGatewayChanged)
    Q_PROPERTY(QString primaryDns READ primaryDns WRITE setPrimaryDns NOTIFY primaryDnsChanged)
    Q_PROPERTY(QString secondaryDns READ secondaryDns WRITE setSecondaryDns NOTIFY secondaryDnsChanged)

public:
    explicit Configuration(const QString &configName, QObject *parent = 0);
    virtual ~Configuration();
    
    bool active() const;

    void setUseDhcp(bool use);
    bool isUseDhcp() const;

    void setIpAddress(const QString &ip);
    QString ipAddress() const;

    void setSubnetMask(const QString &mask);
    QString subnetMask() const;

    void setDefaultGateway(const QString &gateway);
    QString defaultGateway() const;

    void setPrimaryDns(const QString &dns);
    QString primaryDns() const;

    void setSecondaryDns(const QString &dns);
    QString secondaryDns() const;

public slots:
    void upInterface();
    void downInterface();

signals:
    void isUseDhcpChanged(bool use);
    void ipAddressChanged(const QString &ip);
    void subnetMaskChanged(const QString &mask);
    void defaultGatewayChanged(const QString &gateway);
    void primaryDnsChanged(const QString &dns);
    void secondaryDnsChanged(const QString &dns);

    void activeChanged();

protected:
    void exec(const QString &command);

    QHash<QString /*command*/, QVariant /*value*/> m_values;

protected slots:
    virtual void handleResponse(const QString &command, const QString &response);

private:
    void updateInterface(const QString &response);
    void updateDhcpEnabled(const QString &command, const QString &response);
    void updateIpAddress(const QString &command, const QString &response);
    void updateSubnetMask(const QString &command, const QString &response);
    void updateDefaultGateway(const QString &command, const QString &response);
    void updatePrimaryDns(const QString &command, const QString &response);
    void updateSecondaryDns(const QString &command, const QString &response);
    void updateInterfaceActivity(const QString &command, const QString &response);

    bool m_active;
    bool m_isUseDhcp;
    QString m_configName;
    QString m_interfaceName;
    QString m_ipAddress;
    QString m_subnetMask;
    QString m_defaultGateway;
    QString m_primaryDns;
    QString m_secondaryDns;

    ConfigHelper *m_configHelper;
};

#endif // CONFIGURATION_H
