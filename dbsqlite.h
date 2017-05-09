#ifndef DBSQLITE_H
#define DBSQLITE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QStringList>
#include <QVariantMap>
#include "fileinfo.h"
typedef enum {
    ACT_NULL,
    ACT_GETSWPNS,
} postProc_t;

typedef enum {
    REP_SYNCED = 1,
    REP_NOTSYNCED,
    REP_NOTREF
} repstat_t;

class DBSqlite : public QObject
{
public:
    DBSqlite(QObject *parent = 0);
    //~DBSqlite();

public:
    bool loadDB(bool clean = false);
    int verifySWPNs(bool remove = false);
    void loadSWPN(bool extras = true);
    int cleanDB();
    void showCounts();

    QStringList & getSwpnList() { return mSwpnList; }
    QString getSoftwareTitle(QString swpn);
    QString getMediaCount(QString swpn);
    QString getCocFormIssuer(QString swpn);
    QString getEstimateLoadTime(QString swpn);
    QString getLoadingStandard(QString swpn);
    QStringList getRepositoryData();

    void openNewSwpn();
    bool nextNewSwpn(int *count, QString &swpn);

    void resetRepInfo();
    void resetRepFound();
    void insertRepFound(QMap<QString, QString> &map, repstat_t repstat);
    void addRepInfo(QString swpn, QString media_count, int status);
    int countTable(QString table);
    QString getSingleValue(QString sql);
    int getValidCount();
    int getSwpnMetaCount() { return mSwpnMetaCount; }

private:
    bool openDB();
    bool closeDB();
    bool deleteDB();
    QSqlError lastError();
    void endNewSwpn();
    void actionGetSwns();
    bool loadNeeded();
    int  oneSQL(QString sql, QString text);

private slots:
    void finishedApp();

private:
    FileInfo mFi;
    QProcess *mProcess;
    postProc_t mPostProcess;
    QStringList mSwpnList;
    QMap<QString, QString> mSwpnsMC;
    QSqlDatabase db;
    QSqlQuery newSwpn;
    QSqlQuery *mpSwpnsRepIns;
    bool checkSWPN(QString swpn, int count);
    void getSWPNs();
    bool loadSqlFile(QString sqlfile);
    int buildDelta();
    void runCommand(QString command);

    int mMissingCount;
    int mSwpnMetaCount;

    QMap<QString, QString> mMissingSwpns;
    QMap<QString, QString> mFoundSwpns;
    QMap<QString, QString> mNotRefSwpns;

    QMapIterator<QString, QString> *mMissingSwpnsIter;
};

#endif // DBSQLITE_H

