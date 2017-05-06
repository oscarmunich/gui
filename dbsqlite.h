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

        QStringList & getSwpnList() { return mSwpnList; };
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
        void insertRepFound();
        void addRepInfo(QString swpn, QString media_count, bool inRep);

private:
        bool openDB();
        bool closeDB();
        bool deleteDB();
        int countTable(QString table);
        QSqlError lastError();
        void endNewSwpn();
        void actionGetSwns();
        bool loadNeeded();

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
        QMap<QString, QString> mMissingSwpns;
        QMapIterator<QString, QString> *mMissingSwpnsIter;

        QMap<QString, QString> mFoundSwpns;
        QMapIterator<QString, QString> *mFoundSwpnsIter;
    };

#endif // DBSQLITE_H

