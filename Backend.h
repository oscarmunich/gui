#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QStringListModel>
#include <QDir>
#include <QMap>
#include <QHash>
#include <QDateTime>
#include <QProcess>
#include <QRegExp>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QMutexLocker>
#include "loadclient.h"
#include "dbsqlite.h"
#include "dbsettings.h"
#include "bthread.h"
#include "syncthread.h"
#include "monitorserver.h"
#include "runclient.h"
#include "swpnmatch.h"
#include "backendbase.h"
#include "loadsyncclient.h"
#include "cvaluesourcebase.h"

#define SYNC_DELAY (6000 * 1)

struct Backend_SoftwareInfo
{
    int avgLoad;
    int noMedia;
    QString title;
    QDateTime added;
};

typedef enum {
    EXUSB_EXCEPTION,
    EXUSB_ERROR,
} usb_reset_t;

// Warning order must follow above enum (Change);
typedef enum {
        COL_WHITE,
        COL_RED,
        COL_YELLOW,
        COL_GREEN,
        COL_BLUE
} colors_t;

#define IFDBG(x) if (gBackend->ifDebug(x))

#define IFDBGALL IFDBG(DBG_ALL)
#define IFDBGSQL IFDBG(DBG_SQL)
#define IFDBGREP IFDBG(DBG_REP)
#define IFDBGCOM IFDBG(DBG_COM)

typedef enum {
    DBG_ALL = 0x0001,
    DBG_SQL = 0x0002,
    DBG_REP = 0x0004,
    DBG_COM = 0x0008
} debugws_t;

class Backend;
extern Backend *gBackend;

class Backend : public BackendBase
{
    Q_OBJECT
// DB Related changes
private:
    DBSqlite db;
    DBSettings settingsdb;
    QHash <QString, int> mColHash = {
        {"COMP",  COL_BLUE},
        {"ABORT", COL_RED},
        {"HRDW",  COL_RED},
        {"R/W",   COL_RED},
        {"XREF",  COL_RED}
    };


// Thread stuff
private:
    SyncThread *mSslThread;
    QThread *mThread;
    BThread *mWorker;
    RunClient mRunClient;

    void createSslThread();
    void createThread();
    void setupConnections();

public slots:
    void PNOk(QString pn, bool pnok);
    void errorString(QString error) {
        qDebug() << "errorString" << error;
    }
// End of thread stuff
// PrepareSUB stuff
public slots:
    void commandResponse(int retcode, QString output);
    void commandOutput(QString output);

// Signal to SSL
signals:
    void synchroniseWithServer();
    void checkPN(QString pn);

private:
    QApplication *mApp;

signals:
    void sendAString(QString string);

public:
    DBSqlite & getDB()           { return db;}
    DBSettings & getSettingsDB() { return settingsdb;}
    void setAccessKey(QString accessKey) { mAccessKey = accessKey; }
    QString getAccessKey()       {
        //if (mAccessKey.isEmpty()) {
            mAccessKey = "be49325e9de0814c58d94efee4be5f5b987ec8c3";
        //}
        return mAccessKey;
    }
private:
    QString mAccessKey;
    swpnMatch mSwpnMatch;
    QString mChoices;
    loadSyncClient mSyncClient;

public:
    explicit Backend(QObject *parent = 0);

    void syncFinished();
    void syncEnd();

    void Init();
    QStringListModel *airlineModel();
    QStringListModel *actypeModel();
    QStringListModel *tailsignModel();
    QStringListModel *softwareModel();

    QString getSWPath() {
        QString t;
        return t.sprintf("%s/SW/%s", (const char*) mAirlineKey.toUtf8(), (const char*) mSoftware.toUtf8());
    }
    Q_INVOKABLE QString doCheckTail(QString in, bool valid);
    Q_INVOKABLE QString doCheckSwpn(QString in, bool valid);
    Q_INVOKABLE QString getChoices();
    Q_INVOKABLE void setData(QString datatype, QString value);
    Q_INVOKABLE void run6153load();
    Q_INVOKABLE void run615aload();
    Q_INVOKABLE void runusbload();

    QStringList mColors = { "white", "red", "yellow", "lightgreen", "blue" };

    QRegExp mRETailSignWhole;
    QRegExp mRETailSignStart;

signals:
    void statusChanged();
    void statusColorChanged();
    void showHomeEnabledChanged();
    void showServerIdChanged();
    void showServerNameChanged();
    void pdlNameChanged();
    void pageChanged();
    void flsDeskChanged();
    void eescmChanged();
    void screenStateChanged(bool turnedOn);

 public slots:

    void setConfigValue(const QString a, const QString b);

    void runCommand(QString command);

    void setAirline(QString name);
    void setACType(QString name);
    void setTailsign(QString name);
    void setAtaChapter(QString name);
    void setAtaSection(QString name);
    void setSoftware(QString name);
    void setSoftwareTitle(QString name);
    void setLastSync(int tm);

    void setStatus(QString status);
    void reloadData();

    void startDataLoad();
    void stopDataLoad();
    void dataLoadStopped();

    void creatMonitorSocket();
    void updateElapsed();
    void updateConnectionType();

    void syncButtonPressed();
    void startSynchronisation();
    void stopSynchronisation();

    void startSynchroniseWithServer();

    void quit();
    void restart();
    void shutdown();
    void resetAll();
    void resetSome();
    void startSync();
    void connRemoteSuport();
    void purgeRepository();

    void addLog(QString line);
    void setRunning(bool running);
    void setUpdating(bool updating);

    void setRemainingTime(QString RemainingTime);
    void setFLSDesk(bool bFLSDesk);
    void setEESCM(bool bEESCM);

    bool getA6153Enabled();
    bool getA615AEnabled();
    bool getSecureUsbEnabled();

    QStringList getRepositoryData();

    void switchScreen(bool onoff);
    void turnOffScreen();
    bool isDebugMode();
    bool isDemoPDLMode() { return mDemoPDLMode; }

private:
    QMutex mMutex;
    QTimer mTimer;
    QTimer mAppTimer;
    QTimer mTimerSync;

private slots:
    void timeout();

protected slots:
    void setShowLogWindow(bool showWindow);
public slots:
    void setHomeEnabled(bool enabled);
    void setServerId(int ServerId);
    void setServerName(QString ServerName);

    void clearLog();
public:
    void setSyncTimer(int syncTime)    { mSyncTimer    = syncTime; }
    void setSyncTimerExc(int syncTime) { mSyncTimerExc = syncTime; }
    void writeStringToFile(const char * file, const char *data);

    void setPower(bool power);
    bool isPower();

    int mDebugSetting;
    bool ifDebug(debugws_t dbg) {
        return (mDebugSetting & (DBG_ALL || dbg) ? true : false);
    }

    MonitorServer *mMonServer;
    FileInfo mFi;

    swpnval_t getSwpnState() {
        //qDebug() << "getSwpnState" << (int) mSwpnVal;
        return mSwpnVal;
    }

    swpnval_t getTailState() {
        //qDebug() << "getTailState" << (int) mTailVal;
        return mTailVal;
    }

    Q_INVOKABLE QString getStatusColorString() {
        QString color = mColors[mColHash.value(mStatus, COL_BLUE)];
        //qDebug() << "getStatusColor" << color;
        return color;
    }

    Q_INVOKABLE QString getSwpnStateColor() {
        QString color = mColors[(int)getSwpnState()];
        //qDebug() << "getSwpnState" << color;
        return color;
    }

    Q_INVOKABLE QString getTailStateColor() {
        QString color = mColors[(int)getTailState()];
        //qDebug() << "getTailState" << color;
        return color;
    }

  private:
    QProcess *mProcess;
    void checkManualComplete();
    QString mManSoftware;
    QString mManTailSign;
    LoadClient load;
    QString mAirlineKey;
    QTime   mElapseTimer;

    QString mStart;
    QString mRemainingTime;

    QStringListModel *mAirlineModel;
    QStringListModel *mAcTypeModel;
    QStringListModel *mTailsignModel;
    QStringListModel *mSoftwareModel;

    QMap<QString, QString>     Airlines;
    QMap<QString, QStringList> Aircraft_Type;
    QMap<QString, QStringList> Aircraft_Tailsign;
    QMap<QString, QStringList> Type_Software;

    QStringList All_Software;

    QDir mWorkdir;
    bool mSyncAllowed;
    bool mPower;
    bool mPreparing;
    bool mRunning;
    bool mInAbort;
    int  mCompleteTime;
    int  mSyncTimerExc;
    int  mSyncTimer;
    int  mServerId;
    QString mServerName;

    swpnval_t mTailVal;
    swpnval_t mSwpnVal;

    QStringList mPages;
    QString mCurrPage;

    QString mEmpty;

    QString getBuildInfo();
    QString getStartTime();
    QString getRemainingTime();
    QString getElapsedTime();
    QString getConnectionType();
    QString getProgress();

    int  getServerId();
    QString  getServerName();
//    bool getUsbButtonEnabled();
    bool getHomeEnabled();
    bool getShowLogWindow();
//    bool getShowUSBAbort();
    bool getInstallRunning();
    bool getShowAdministration();

    bool getFLSDesk();
    bool getEESM();
    bool getDemoPDLMode();

    QString getPdlName();
    QString getLastSync();
    QString getUpdateStatus();
    QString getUpdateStartTime();
    QString getUpdateProgress();

    QString getUploadStatus();

    QString getDisk();
    QString getDisks();
    QString getFile();
    QString getBlock();
    QString getBlocks();

    Q_PROPERTY(QString status        READ getStatus                               NOTIFY statusChanged)
    Q_PROPERTY(QString statusColor   READ getStatusColor                          NOTIFY statusColorChanged)
};

#endif // BACKEND_H
