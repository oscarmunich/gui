#ifndef FILEINFO_H
#define FILEINFO_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include "config.h"

#define PDLDIR  "/home/root/loader"
#define MACDIR  "/Users/kevin/PDL_dev/PDL_home/home/root/loader"
#define LINDIR  "/home/kevin/loader"

class FileInfo : public QObject
{
    Q_OBJECT
public:
    explicit FileInfo(QObject *parent = 0);

signals:
    
public slots:

public:
    static QString mTestPath;
    void check4TestDir();

    bool setPath(const QString &path, bool pdl);
    QString getRepositoryDir() {
        return mPath + "/Repository";
    }

    QString getRepositorySWDir() {
        return mPath + "/Repository/SW";
    }

    QString getDataFileName() {
        return mPath + "/DB/sqlinput.txt";
    }

    QString getSyncTimeFile() {
        return mPath + "/DB/syncTime";
    }

    QString getValidSwpns() {
        return mPath + "/DB/validswpns.txt";
    }

    QString getDataBaseName() {
        return mPath + "/DB/sql.db";
    }

    QString getDataBasePath() {
        return mPath + "/DB";
    }

    QString getSettingsFileName() {
        return mPath + "/DB/sqlsettings.txt";
    }

    QString getSettingsDataBaseName() {
        return mPath + "/DB/settingssql.db";
    }

    QString getWorkFileName() {
        return mPath + "/DB/swpns.txt";
    }

    QString getSwpnVerifiedName() {
        return mPath + "/DB/.swpns_verified";
    }

    void setVerified(bool verified) {
        QFile f(getSwpnVerifiedName());
        if (verified) {
            f.open(QIODevice::WriteOnly);
            f.close();
            return;
        }
        f.remove();
    }

    bool isVerified() {
        QFile f(getSwpnVerifiedName());
        return (f.exists()) ? true : false;
    }

    QString getLoadsDir() {
        return mPath + "/Loads";
    }

    bool isSoftwareInstalled(QString checksum) {
        QString spath = mPath + "/SoftwareUpdates/installed";
        QDir d(spath);
        if (!d.exists())
            d.mkpath(spath);

        spath += "/";
        spath += checksum;
        qDebug() << "isSoftwareInstalled" << spath;
        QFile f(spath);
        return (f.exists());
    }

    QString getSWUPdatesDir() {
        QString spath = mPath + "/SoftwareUpdates/" +
                QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmmss");
        QDir d(spath);
        if (!d.exists())
            d.mkpath(spath);

        return spath;
    }

    QString getNetConfigServerName() {
        return "perl " + getWorkDir() + "/configuration_server.pl ";
    }

    QString getUtilName() {
        return "perl " + getWorkDir() + "/utils.pl ";
    }

    QString getScriptName() {
        return "perl " + getWorkDir() + "/procMessage.pl ";
    }

    QString getLoaderName() {
        return "perl " + getWorkDir() + "/connect.pl ";
    }

    QString getUSBScript() {
        return "perl " + getWorkDir() + "/usb.pl ";
    }

    QString getWorkDir() {
        return mPath + "/scripts";
    }

    QString getConfigsDir() {
        return mPath + "/scripts";
    }

    QString getTempDir() {
        return mPath + "/tmpdir";
    }

    bool isDemoMode() {
        QFile f(mPath + "/.demo");
        return f.exists();
    }

    QString getOrigDir() {
        return mPath + "/.orig";
    }

    QString getSaveDir() {
        return mPath + "/.save";
    }

    QString getCertName() {
        return mPath + "/certificate/" + gConfig->getCert();
    }

    QString getTimeString() {
        QDateTime t(QDateTime::currentDateTime());
        return t.toString("MMddhhmmss");
    }

    QString getTempName(QString name = "tmp") {
        return getTempDir() + "/" + getTimeString() + "_" + name;
    }

    QString getPath() {
        return mPath;
    }

    bool isOnPDL() {
        return mOnPDL;
    }

    bool isAbortTest() {
        QFile f(mPath + "/.aborttest");
        return f.exists();
    }

    bool isFreeMode() {
        QFile f(mPath + "/.free");
        return f.exists();
    }

    bool isDebugMode() {
        QFile f(mPath + "/.debug");
        return f.exists();
    }

    bool useWatchDog() {
        QFile f(mPath + "/.watchdog");
        return f.exists();
    }

    bool useProxy() {
        QFile f(mPath + "/.useproxy");
        return f.exists();
    }

    bool isStunnelMode() {
        QFile f(mPath + "/.stunnel");
        return f.exists();
    }

    bool noMetaGzip() {
        QFile f(mPath + "/.nogzip");
        return f.exists();
    }

    void inSync(bool sync) {
        QFile f("/tmp/.state_sync");
        if (sync) {
            f.open(QIODevice::WriteOnly);
            f.close();
            return;
        }
        f.remove();
    }

    int getAutoSync() {
        QFile f(mPath + "/.autosync");
        if (!f.exists())
            return 300000; // default to 5 minutes
        int rval;
        QByteArray idata;
        char data[10];
        f.open(QIODevice::ReadOnly);
        f.read(data, 10);
        f.close();
        for (int i = 0; i < 5; i++) {
            if (data[i] < '0' or data[i] > '9')
                break;
            idata.append(data[i]);
        }
        rval = idata.toInt() * 1000;
        qDebug() << "getAutoSync" << rval << idata;
        return rval;
    }

    int getConnection() {
        QFile f(mPath + "/.connection");
        if (!f.exists())
            return 0; // default to basic
        int rval;
        QByteArray idata;
        char data[10];
        f.open(QIODevice::ReadOnly);
        f.read(data, 10);
        f.close();
        for (int i = 0; i < 5; i++) {
            if (data[i] < '0' or data[i] > '9')
                break;
            idata.append(data[i]);
        }
        rval = idata.toInt();
        qDebug() << "getConnection" << rval << idata;
        return rval;
    }

    void setConnection(int rval) {
        QFile f(mPath + "/.connection");

        char data[10];
        sprintf(data, "%d\n", rval);

        f.open(QIODevice::WriteOnly);
        f.write(data, strlen(data));
        f.close();

        qDebug() << "setConnection" << rval;
    }

    void inLoad(bool sync) {
        QFile f("/tmp/.state_load");
        if (sync) {
            f.open(QIODevice::WriteOnly);
            f.close();
            return;
        }
        f.remove();
    }

    int getPort();

    QString mPath;
    bool mOnPDL;
};

#endif // FILEINFO_H


