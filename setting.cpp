#include "setting.h"
#include "fileinfo.h"
#include <QFile>
#include <QDir>
#include <QDebug>

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

Setting::Setting(QObject *parent) :
    QObject(parent)
{
}
//INSERT INTO vw_pdl_pdlsetting VALUES ("3G_enabled", "1");
//"3G_enabled", "1";"3G_roaming_enabled", "1";"a6153_enabled", "1";"a615a_enabled", "0";"dns1", "8.8.8.8";"dns2", "8.8.4.4";"lan_dhcp", "1";"lan_enabled", "1";"lan_gateway", "";"lan_ip", "";"lan_subnet", "";"pdl_name", "mbs Demo PDL 024";"secure_usb_enabled", "1";"sync_interval", "15";"sync_server", "212.227.98.45";"wifi_enabled", "0";"wifi_network1_auth", "";"wifi_network1_password", "";"wifi_network1_ssid", "";"wifi_network2_auth", "";"wifi_network2_password", "";"wifi_network2_ssid", "";"wifi_network3_auth", "";"wifi_network3_password", "";"wifi_network3_ssid", "";"wifi_network4_auth", "";"wifi_network4_password", "";"wifi_network4_ssid", "";"wifi_network5_auth", "";"wifi_network5_password", "";"wifi_network5_ssid", "";
#define TESTSTR "\"3G_enabled\", \"1\";\"3G_roaming_enabled\", \"1\";\"a6153_enabled\", \"1\";\"a615a_enabled\", \"0\";\"dns1\", \"8.8.8.8\";\"dns2\", \"8.8.4.4\";\"lan_dhcp\", \"1\";\"lan_enabled\", \"1\";\"lan_gateway\", \"\";\"lan_ip\", \"\";\"lan_subnet\", \"\";\"pdl_name\", \"mbs Demo PDL 024\";\"secure_usb_enabled\", \"1\";\"sync_interval\", \"15\";\"sync_server\", \"212.227.98.45\";\"wifi_enabled\", \"0\";\"wifi_network1_auth\", \"\";\"wifi_network1_password\", \"\";\"wifi_network1_ssid\", \"\";\"wifi_network2_auth\", \"\";\"wifi_network2_password\", \"\";\"wifi_network2_ssid\", \"\";\"wifi_network3_auth\", \"\";\"wifi_network3_password\", \"\";\"wifi_network3_ssid\", \"\";\"wifi_network4_auth\", \"\";\"wifi_network4_password\", \"\";\"wifi_network4_ssid\", \"\";\"wifi_network5_auth\", \"\";\"wifi_network5_password\", \"\";\"wifi_network5_ssid\", \"\";"

void Setting::dump2File() {
    buildSqlStart();
    char buf[256];
    QHashIterator<QString, QString> qi(mInfoMap);

    while (qi.hasNext()) {
        qi.next();

        SNPRINTF(buf, 256, "INSERT INTO vw_pdl_pdlsetting VALUES (\"%s\", \"%s\");", (const char*)qi.key().toUtf8(), (const char*)qi.value().toUtf8());
        mSql.append(QString::fromLatin1(buf));
    }
    buildSqlEnd();
    buildSettingsFile();
}

void Setting::test() {
    QString input(TESTSTR);
    saveSettings(input);
}

void Setting::saveSettings(const QString & input) {
    QStringList l = input.split(';');
    for (int i=0; i < l.count(); i++) {
        //qDebug() << l[i];
        QStringList item = l[i].split(", ");
        if (item.count() != 2)
            continue;
        item[0].replace(QString("\""), QString(""));
        item[1].replace(QString("\""), QString(""));
        qDebug() << "I" << item[0] << item[1];
        mInfoMap[item[0]] = item[1];
    }
}

void Setting::setValue(const QString & key, const QString & value) {
    mInfoMap[key] = value;
}

bool Setting::buildSettingsFile() {
    FileInfo mFi;
    QString file = mFi.getSettingsFileName();
    QFile f(file);

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open:<" + file + ">";
        return false;
    }

    for (int i=0; i < mSql.count(); i++) {
        mSql[i] += "\n";
        f.write((const char*)mSql[i].toUtf8());
    }
    f.close();
    return true;
}

void Setting::buildSqlStart() {
    mSql.clear();

    mSql.append("DROP TABLE IF EXISTS vw_pdl_pdlsetting;");
    mSql.append("CREATE TABLE vw_pdl_pdlsetting ( setkey, setvalue );");
    mSql.append("VACUUM;");
    mSql.append("PRAGMA foreign_keys=OFF;");
    mSql.append("BEGIN TRANSACTION;");
}

void Setting::buildSqlEnd() {
    mSql.append("COMMIT;");
}
