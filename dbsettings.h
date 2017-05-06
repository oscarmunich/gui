#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QHash>
#include <QDebug>
#include <QSqlQuery>
#include <QStringList>


/*
 ("pdl_name", "easyPDL-01");
 ("lan_enabled", "1");
 ("lan_dhcp", "0");
 ("lan_ip", "172.16.0.54");
 ("lan_subnet", "255.255.255.0");
 ("lan_gateway", "172.16.0.1");
 ("dns1", "8.8.8.8");
 ("dns2", "8.8.4.4");
 ("wifi_enabled", "1");
 ("wifi_network1_ssid", "mbs wifi");
 ("wifi_network1_auth", "WPA");
 ("wifi_network1_password", "dataload");
 ("wifi_network2_ssid", "");
 ("wifi_network2_auth", "");
 ("wifi_network2_password", "");
 ("wifi_network3_ssid", "");
 ("wifi_network3_auth", "");
 ("wifi_network3_password", "");
 ("wifi_network4_ssid", "");
 ("wifi_network4_auth", "");
 ("wifi_network4_password", "");
 ("wifi_network5_ssid", "");
 ("wifi_network5_auth", "");
 ("wifi_network5_password", "");
 ("3G_enabled", "1");
 ("3G_roaming_enabled", "1");
 ("a6153_enabled", "1");
 ("a615a_enabled", "0");
 ("secure_usb_enabled", "1");
 ("update_server", "www.mbs-electronics.com");
 ("update_intervall_minutes", "10");
 ("update_timestamp", "1368619474");
 ("dataloader_version", "1001");
 */
class DBSettings : public QObject
    {
    public:
        DBSettings(QObject *parent = 0);
        //~DBSettings();

    public:
        bool openDB();
        bool loadDB();
        bool closeDB();
        bool deleteDB();
        QSqlError lastError();

        bool insertKey(const QString &key);
        bool setValue(const QString &key, const QString &val);
        bool setValueInt(const QString &key, int value);
        bool getValueInt(const QString &key, int *value) {
            QString tvalue;
            if (!getDBValue(key, tvalue)) {
                //qDebug() << "getValueInt:Not found" << key;
                return false;
            }
            *value = tvalue.toInt();
            //qDebug() << "getValueBool:" << *value;
            return true;
        }

        void setTime();
        void checkForLocal();
        bool getValueBool(const QString &key, bool *value) {
            int tvalue;
            if (!getValueInt(key, &tvalue)) {
                //qDebug() << "getValueBool:Not found" << key;
                return false;
            }
            *value = (tvalue) ? true : false;
            //qDebug() << "getValueBool:" << tvalue;
            return true;
        }
        bool get_lan_enabled(bool *value)              { return getValueBool("lan_enabled", value); }
        bool get_lan_dhcp(bool *value)                 { return getValueBool("lan_dhcp", value); }
        bool get_wifi_enabled(bool *value)             { return getValueBool("wifi_enabled", value); }
        bool get_3G_enabled(bool *value)               { return getValueBool("3G_enabled", value); }
        bool get_3G_roaming_enabled(bool *value)       { return getValueBool("3G_roaming_enabled", value); }
        bool get_a6153_enabled(bool *value)            { return getValueBool("a6153_enabled", value); }
        bool get_a615a_enabled(bool *value)            { return getValueBool("a615a_enabled", value); }
        bool get_secure_usb_enabled(bool *value)       { return getValueBool("secure_usb_enabled", value); }

        bool get_lan_ip(QString &value)                { return getValueString("lan_ip", value); }
        bool get_lan_subnet(QString &value)            { return getValueString("lan_subnet", value); }
        bool get_lan_gateway(QString &value)           { return getValueString("lan_gateway", value); }
        bool get_update_server(QString &value)         { return getValueString("update_server", value); }
        bool get_sync_server(QString &value)           { return getValueString("sync_server", value); }

        bool get_update_intervall_minutes(int *value)  { return getValueInt("update_intervall_minutes", value); }
        bool get_update_timestamp(int *value)          { return getValueInt("update_timestamp", value); }
        bool get_dataloader_version(int *value)        { return getValueInt("dataloader_version", value); }

        bool getFile(QString file, QByteArray & value);
        void write2File(QString file, QString value);
        bool set_update_timestamp(int value);
        bool get_pdl_name(QString &value);
        bool set_db_time(QString value);
        bool set_db_pdl_change_time(QString value);

        void load_update_timestamp();
        void load_pdl_name();
        bool getWifiNetworkInfo(const QString &ssid, QString &auth, QString &pass) {
            QString rssid;
            for (int i=1; i <= 5; i++) {
                if (!getWifiNetwork(i, rssid, auth, pass))
                    continue;
                if (ssid != rssid)
                    continue;
                return true;
            }
            return false;
        }

        bool getWifiNetwork(int no, QString &ssid, QString &auth, QString &pass) {
            QString t;
            getDBValue(t.sprintf("wifi_network%d_ssid",     no), ssid);
            getDBValue(t.sprintf("wifi_network%d_auth",     no), auth);
            getDBValue(t.sprintf("wifi_network%d_password", no), pass);
            return (auth.isEmpty()) ? false : true;
        }

        bool getDNSServers(QString &dns1, QString &dns2) {
            QString t;
            getDBValue("dns1", dns1);
            getDBValue("dns2", dns2);
            return (dns1.isEmpty() && dns1.isEmpty()) ? false : true;
        }

        bool getValueString(const QString &key, QString &value) {
            return getDBValue(key, value);
        }
        bool getDBValue(const QString &key, QString &value);

    private:
        void dumpDB();
        void hashDB();
        bool loadDBFromFile();
        QSqlDatabase db;
        QHash<QString, QString> mdbinfo;
    };

#endif // DBSETTINGS_H

