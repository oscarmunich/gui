#include <QApplication>
#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include "Backend.h"
#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QXmlStreamReader>
#include "fileinfo.h"
#include "config.h"
#include <QTimer>
#include <QMessageBox>
#include "dbsettings.h"
#include "./helpers/confighelper.h"
#include "mediainfo.h"
#include "checkrepository.h"
#include "syncthread.h"
#include "build_number.h"
#include "checkrepository.h"

#ifdef Q_WS_WIN
#include <windows.h>
#include <winbase.h>
#endif

/* Global values used in GUI */
extern QmlApplicationViewer *gViewer;


Backend::Backend(QObject *parent) :
    BackendBase(parent)
{
}

void Backend::Init()
{
    gBackend = this;

    mAirlineModel  = new QStringListModel(this);
    mAcTypeModel   = new QStringListModel(this);
    mTailsignModel = new QStringListModel(this);
    mSoftwareModel = new QStringListModel(this);

    mStatus = "RDY";

    setStatusColor("blue");

    creatMonitorSocket();

    connect(&mRunClient,              SIGNAL(setStatus(QString)),            this, SLOT(setStatus(QString)));
    connect(&mRunClient,              SIGNAL(commandResponse(int, QString)), this, SLOT(commandResponse(int, QString)));
    connect(&mRunClient,              SIGNAL(commandOutput(QString)),        this, SLOT(commandOutput(QString)));

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    mTimer.start(1000);

    switchScreen(true);

    CheckRepository::createCheckThread();

    createSslThread();

    QString buildinfo;
    buildinfo  += BUILDDATE;
    buildinfo  += " ";
    buildinfo  += BUILDTIME;
    buildinfo  += " (";
    buildinfo  += BUILD;
    buildinfo  += ")";
    setBuildInfo(buildinfo);

    qDebug() << "Build info" << BUILD << BUILDDATE << BUILDTIME;

    connect(ConfigHelper::instance(), SIGNAL(reply(QString, QString)),       this, SLOT(setConfigValue(QString, QString)));
    ConfigHelper::instance()->execute("C GetAccessKey");
    ConfigHelper::instance()->execute("C GetDemoMode");
}

void Backend::initialiseDatabases(bool something)
{
    qDebug() << "initialiseDatabases -----------------------------------------";
    getSettingsDB().loadDB();

    bool bv;
    getSettingsDB().get_a6153_enabled(&bv);

    QString t;
    getSettingsDB().get_pdl_name(t);
    setPdlName(t);

    int tm;
    getSettingsDB().get_update_timestamp(&tm);
    setLastSync(tm);

    // Load and clean SWPNS.
    mFi.setVerified(false);
    getDB().loadDB();
    getDB().resetRepFound();
    getDB().verifySWPNs(true);
    loadRepositoryCounts();
}

void Backend::setConfigValue(const QString a, const QString b)
{
    qDebug() << "Backend::setConfigValue" << a << b;
    if (a == "GetAccessKey") {
        mAccessKey=b;
    }
}

void Backend::loadRepositoryCounts() {
    int availswpns     = getDB().countTable("vw_pdl_swpn");
    int notdownloaded  = getDB().getSingleValue("select count(*) from tmp_swpn_rep where status = 2").toInt();
    setRepositoryAll(availswpns + notdownloaded);
    setRepositoryValid(getDB().getValidCount());
    setRepositoryNotDown(getRepositoryAll() - availswpns);
    setRepositoryExpired(availswpns - getRepositoryValid());
    setRepositoryNotRef(getDB().getSingleValue("select count(*) from tmp_swpn_rep where status = 3").toInt());
}

// ---------------- SWPN selection
QStringListModel *Backend::airlineModel()
{
    return mAirlineModel;
}

QStringListModel *Backend::actypeModel()
{
    return mAcTypeModel;
}

QStringListModel *Backend::tailsignModel()
{
    return mTailsignModel;
}

QStringListModel *Backend::softwareModel()
{
    return mSoftwareModel;
}

void Backend::setAirline(QString name)
{
    mAirline = name;
    mAirlineKey = Airlines.key(name);
    if (!name.isEmpty()) qDebug() << "setAirLinea:" + name;

    setACType(mEmpty);

    mAcTypeModel->setStringList( Aircraft_Type[mAirlineKey] );
    mAcTypeModel->sort(0);

    emit airlineChanged();
}

void Backend::setACType(QString name)
{
    mACType = name;

    setTailsign(mEmpty);
    mTailsignModel->setStringList( Aircraft_Tailsign[mAirlineKey + "-" + name] );
    mTailsignModel->sort(0);

    emit ACTypeChanged();
}

void Backend::setTailsign(QString name)
{
    mTailsign = name;

    setAtaChapter(mEmpty);

    emit tailsignChanged();
}

void Backend::setAtaChapter(QString name)
{
    mAtaChapter = name;
    setAtaSection(mEmpty);

    emit ataChapterChanged();
}

void Backend::setAtaSection(QString name)
{
       mAtaSection = name;
       setSoftware(mEmpty);

       emit ataSectionChanged();
}

void Backend::setSoftware(QString name)
{
    qDebug() << "setSoftware" << name;
    if (name.isEmpty()) {
        mSoftware = name;
        setSoftwareTitle(mEmpty);
        setSoftwareCoC(mEmpty);
        setRemainingTime(mEmpty);
        setA6153load(false);
        setA615aload(false);
        setUsbload(false);
        emit softwareChanged();
    }

    resetSome();
    mSoftware = name;
    setSoftwareTitle(db.getSoftwareTitle(name));
    setSoftwareCoC(db.getCocFormIssuer(name));
    QString ls = db.getLoadingStandard(name);
    qDebug() << "Loading standard:" << ls;
    QStringList sl = ls.split(',');
    while(sl.size() > 0) {
        ls = sl.front(); sl.pop_front();
        qDebug() << "standard:" << ls;
        if (ls == "A615-3") {
            setA6153load(true);
            continue;
        }
        if (ls == "A615-A") {
            setA615aload(true);
            continue;
        }
        if (ls == "USB") {
            //setUsbload(true);
            continue;
        }
    }
    setDisk(db.getMediaCount(name));
    setRemainingTime(db.getEstimateLoadTime(name));
    emit softwareChanged();
}

void Backend::setSoftwareTitle(QString name)
{
    mSoftwareTitle = name.left(32);
    emit softwareTitleChanged();
}

// ----------------------------------------------------------

void Backend::setStatus(QString status)
{
    mStatus = status;
    setStatusColor(getStatusColorString());
    emit statusChanged();
}

void Backend::setLastSync(int tm)
{
    QDateTime dt;
    qDebug() << "setLastSync" << tm;
    getSettingsDB().set_update_timestamp(tm);
    dt.setTime_t(tm);
    mLastSync = dt.toString("yyyy-MM-dd hh:mm");
    emit lastSyncChanged();
}

void Backend::writeStringToFile(const char * file, const char *data) {
    QFile f(file);
    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open:" << file;
        return;
    }
    f.write(data, strlen(data));
    f.close();
}

void Backend::turnOffScreen() {
    emit switchScreen(false);
}

void Backend::switchScreen(bool onoff) {
    qDebug() << "########## switchScreen:" << onoff;
//    QDir d("/sys/class/gpio/gpio13");
//    if (!d.exists()) {
//        writeStringToFile("/sys/class/gpio/export", "13");
//    }
//    writeStringToFile("/sys/class/gpio/gpio13/direction", (onoff) ? "in\n" : "out\n");

    emit screenStateChanged(onoff);
}

void Backend::setPower(bool power)
{
    mPower = power;
    switch((mPower) ? 1 : 0) {
    case 1:
        //sleep(1);
        //gViewer->show();
        break;
    case 0:
        //gViewer->hide();
        break;
    }
}

bool Backend::isPower()
{
    return mPower;
}

void Backend::timeout()
{
    updateElapsed();
}

void Backend::updateElapsed()
{
    if (!getSyncRunning())
        return;

    QTime elapsed(0,0);
    QString e = elapsed.addMSecs(mElapseTimer.elapsed()).toString("hh:mm:ss");
    setElapsedTime(e);
}

void Backend::updateConnectionType()
{
}

// ------ Synchronisation --------------------
void Backend::syncButtonPressed()
{
    QMutexLocker l(&mMutex);
    if (getSyncRunning()) {
        stopSynchronisation();
    } else {
        startSynchronisation();
    }
}


// Start the synchronisation
void Backend::startSynchronisation()
{
    setSyncButtonText("Stop Synchronisation");

    resetSome();
    setShowLogWindow(true);

    setUpdateStartTime(QTime::currentTime().toString("HH:mm:ss"));
    mElapseTimer.start();
    setUpdateProgress("0%");
    setConnectionType("Ethernet");
    setUpdateStatus("Running");

    //QTimer::singleShot(100, this, SLOT(startSynchroniseWithServer()));
    setUpdating(true);
    setHomeEnabled(false);
    gBackend->mFi.setVerified(false);
    mSyncClient.runCommand("");
}

void Backend::startSynchroniseWithServer()
{
    qDebug() << "startSynchroniseWithServer";
    emit synchroniseWithServer();
}

void Backend::syncFinished()
{
    setUpdateStatus("Complete");
    syncEnd();
}
void Backend::stopSynchronisation()
{
    setUpdateStatus("ABORTED");
    syncEnd();
}

void Backend::syncEnd()
{
    setSyncButtonText("Start Synchronisation");
    setHomeEnabled(true);
    ConfigHelper::instance()->execute("C GetRepositoryInfo");
    setUpdating(false);
    initialiseDatabases();
}

// -----------------------------------------------------------

void Backend::runCommand(QString command)
{
    if (mProcess)
        mProcess->deleteLater();

    mProcess = new QProcess(this);

    /* Process start will start the app in this object */
    qDebug() << "Running:" + command;
    mProcess->start(command);
}

void Backend::resetAll()
{
    qDebug() << "resetAll";
    setAirline("");
    setACType("");
    setTailsign("");
    setAtaChapter("");
    setAtaSection("");
    setSoftware("");
    resetSome();
}

void Backend::resetSome()
{
    clearLog();
    setShowLogWindow(false);
    setUpdateProgress("");
    setUpdateStatus("");
    setStatus("");
    setUpdateStartTime("");
    setUploadStatus("");
    setUpdating(false);
    setRemainingTime("");
    setElapsedTime("");
    setDisk("");
    setDisks("");
    setFile("");
    setBlock("");
    setBlocks("");
    setA6153load(false);
    setA615aload(false);
    setUsbload(false);
}

void Backend::setData(QString datatype, QString value)
{
    qDebug() << "setData" << datatype << value;
    if (datatype == "Airline") {
        setAirline(value);
        return;
    }
    if (datatype == "ACType")  {
        setACType(value);
        return;
    }
    if (datatype == "Tailsign") {
        setTailsign(value);
        return;
    }
    if (datatype == "ATA Chapter") {
        setAtaChapter(value);
        return;
    }
    if (datatype == "ATA Section") {
        setAtaSection (value);
        return;
    }
    if (datatype == "Software") {
        setSoftware(value);
        return;
    }
    if (datatype == "Software title") {
        setSoftwareTitle(value);
        return;
    }
}

void Backend::run6153load()
{
    qDebug() << "run6153load";
}

void Backend::run615aload()
{
    qDebug() << "run615aload";
}

void Backend::runusbload()
{
    qDebug() << "runusbload";
}

void Backend::quit()
{
    qDebug() << "quit";
    QString t = mFi.getUtilName() + "-s";
    runCommand(t);
}

QString Backend::doCheckTail(QString in, bool valid) {
    setTailsign(in);
    mTailVal = (in.length() == 0) ? SWPN_NOTSET : SWPN_VALID;
    checkManualComplete();
    return in;
}

void Backend::checkManualComplete() {
    qDebug() << "checkManualComplete" << mSwpnVal << mTailVal << mManSoftware;
    setSoftware((mSwpnVal == SWPN_VALID && mTailVal == SWPN_VALID) ? mManSoftware : "");
}

QString Backend::getChoices() {
    qDebug() << "Choices got:" << mChoices;
    return mChoices;
}

QString Backend::doCheckSwpn(QString in, bool valid) {
    mSwpnVal = (valid) ? SWPN_VALID : SWPN_NOK;
    mManSoftware = in;
    qDebug() << "doCheckSwpn - in " << valid << " " << in;
    setSoftware((valid) ? in : "");
    return in;
}

void Backend::restart()
{
    qDebug() << "Restart" << mRunClient.getState();
    switch (mRunClient.getState()) {
    case RUN_IDLE:
        mRunClient.doRun("/home/kevin/test.sh");
        break;
    case RUN_STARTUP:
        break;
    case RUN_RUNNING:
        mRunClient.stopCommand();
        break;
    }

}

void Backend::shutdown()
{
    return;

    QString t = mFi.getUtilName() + "-s";
    runCommand(t);
}

void Backend::createSslThread()
{
    mSslThread      = new SyncThread;

    mSslThread->start();
}

void Backend::createThread()
{
    mThread = new QThread;
    mWorker = new BThread();
    mWorker->moveToThread(mThread);

    connect(mWorker, SIGNAL(error(QString)),        this,      SLOT(errorString(QString)));
    connect(mThread, SIGNAL(started()),             mWorker,   SLOT(process()));
    connect(this,    SIGNAL(sendAString(QString)),  mWorker,   SLOT(receiveString(QString)), Qt::QueuedConnection);

    mThread->start();

    emit sendAString((QString)"gift");
}

void Backend::connRemoteSuport()
{
    qDebug() << "connRemoteSuport";
    QString t = mFi.getUtilName() + "-c";
    runCommand(t);
}

void Backend::purgeRepository()
{
    QString proc = mFi.getScriptName() + "-P";
    int ret = QProcess::execute(proc);
    qDebug() << "purgeRepository" << proc << ret;
    initialiseDatabases();
    // reset change date.
    getSettingsDB().set_db_pdl_change_time(QString("2000-01-01T00:00:00+00:00"));

    ConfigHelper::instance()->execute("C GetRepositoryInfo");
}

void Backend::stopDataLoad()
{
    if (!mRunning)
        return;

    qDebug() << "STOP DATA LOAD()";
    mStatus = "ABORT";
    addLog("Completed");
    setShowLogWindow(false);
    mFi.inLoad(false);
    setRunning(false);
    load.send2Loader("ABORT");

    setHomeEnabled(true);

    emit statusChanged();
    emit startTimeChanged();
    emit remainTimeChanged();
    emit elapsedTimeChanged();
    emit progressChanged();
}

void Backend::startSync()
{
    qDebug() << "startSync called";
//    startUpload();
}

void Backend::creatMonitorSocket() {
    if (mMonServer) {
        return;
    }
    mMonServer = new MonitorServer();
    bool success = mMonServer->listen(QHostAddress::Any, 4200);
    if(!success) {
        gBackend->addLog("Could not listen on port 4200.");
        //qFatal("Could not listen on port 4200.");
    }

    qDebug() << "creatMonitorSocket";

}

void Backend::dataLoadStopped()
{
    qDebug() << "Dataload stopped";
    addLog("Completed");
    mFi.inLoad(false);
    setRunning(false);
    load.send2Loader("ABORT");

    setHomeEnabled(true);

    emit statusChanged();
    emit startTimeChanged();
    emit remainTimeChanged();
    emit elapsedTimeChanged();
    emit progressChanged();
}

void Backend::commandResponse(int retcode, QString output) {
    qDebug() << "commandResponse" << "retcode" << retcode << "output" << output;
    //emit prepareUSB();
}

void Backend::commandOutput(QString output) {
    qDebug() << "commandOutput" << "output" << output;
}

//#ifdef Q_WS_MAC || Q_WS_DARWIN
//#define USBTEST
//#endif

//void Backend::onAbortUSB()
//{
//    qDebug() << "onAbortUSB";
//    mUSBState = USB_ABORT;
//    prepareUSB();
//}

void Backend::setupConnections() {
    QObject::disconnect(gCheckRep,  0, 0, 0);

    connect(this,       SIGNAL(checkPN(QString)),    gCheckRep,  SLOT(checkPN(QString)),       Qt::QueuedConnection);
    connect(gCheckRep,  SIGNAL(PNOk(QString, bool)), this,       SLOT(PNOk(QString, bool)),    Qt::QueuedConnection);
}

void Backend::PNOk(QString /* pn */, bool pnok) {
    QString mess;
    QString t;
    mess = t.sprintf("%s BB Verification L: %s", (const char*) mSoftware.toUtf8(), (pnok) ? "OK" : "FAILED");
    addLog(mess);
    if (!pnok) {
        addLog("Please abort data load and re-synchronise the PDL");
        return;
    }
//    mUSBState = USB_PNCHECKED;
//    prepareUSB();
}

void Backend :: startDataLoad()
{
    if (mRunning)
        return;

    qDebug() << "START Load";
    mFi.inLoad(true);

    mStatus = "RDY";
    mStart = QTime::currentTime().toString("HH:mm:ss");
    mProgress = "0%";

    if (mFi.isDemoMode()) {
        setShowLogWindow(false);
    } else {
        setShowLogWindow(true);
    }
    clearLog();

    load.synchroniseWithServer(this);
    setRunning(true);

    setHomeEnabled(false);

    emit statusChanged();
    emit startTimeChanged();
    emit remainTimeChanged();
    emit elapsedTimeChanged();
    emit connectionTypeChanged();
    emit progressChanged();

}

QString Backend::getStartTime()
{
    return mStart;
}



QString Backend::getRemainingTime()
{
    return mRemainingTime;
}

//bool Backend::getShowUSBAbort()
//{
//    return mShowUSBAbort;
//}

void Backend::setShowLogWindow(bool showWindow)
{
    //qDebug() << "setShowLogWindow" << showWindow;
    mShowLogWindow = showWindow;
    clearLog();
    emit showLogWindowChanged();
}

//void Backend::setShowUSBAbort(bool show)
//{
//    mShowUSBAbort = show;
//    emit showUSBAbortChanged();
//}

void Backend::setHomeEnabled(bool enable)
{
    mHomeEnabled = enable;
    emit showHomeEnabledChanged();
}

void Backend::setServerId(int serverId)
{
    mServerId = serverId;
    qDebug() << "setServerId" << serverId;
    QString name = "Unknown";
    switch(serverId) {
    case 1:
        name = "eESM Intra (Test)";
        break;
    case 2:
        name = "eESM Intra (Prod)";
        break;
    case 3:
        name = "eESM (Test)";
        break;
    case 4:
        name = "eESM (Prod)";
        break;
    case 5:
        name = "FLS-Desk (Test)";
        break;
    case 6:
        name = "FLS-Desk (Prod)";
        break;
    }
    setServerName(name);
    gConfig->setConfig((conf_type_t)serverId);
    mFi.setConnection(serverId);

    emit showServerIdChanged();
}

void Backend::setServerName(QString serverName)
{
    mServerName = serverName;
    qDebug() << "setServerName" << serverName;
    emit showServerNameChanged();
}

void Backend::setRunning(bool running)
{
    mRunning = running;
    emit installRunningChanged();
}

void Backend::setUpdating(bool updating)
{
    qDebug() << "Backend::setUpdating" << updating;
    setSyncRunning(updating);

    setUploadStatus(""); //KMU
}

void Backend::clearLog()
{
    setLogText("");
    emit showLogWindowChanged();
}

void Backend::addLog(QString line)
{
    QString t = getLogText();
    if (t.isEmpty())
        t = line;
    else
        t += "\n" + line;

    if (t.length() > 1500) {
        t = t.right(1000);
        qDebug() << "######################### trunc log";
    }
    setLogText(t);
 }

void Backend::setRemainingTime(QString remainingTime)
{
    mRemainingTime = remainingTime;
    if (mRemainingTime == "")
        mRemainingTime = "N/A";
    emit remainTimeChanged();
}

bool Backend::getA6153Enabled()
{
    bool result = false;
    getSettingsDB().get_a6153_enabled(&result);
    return result;
}

bool Backend::getA615AEnabled()
{
    bool result = false;
    getSettingsDB().get_a615a_enabled(&result);
    return result;
}

bool Backend::getSecureUsbEnabled()
{
    bool result = false;
    getSettingsDB().get_secure_usb_enabled(&result);
    return result;
}

QStringList Backend::getRepositoryData()
{
    return db.getRepositoryData();
}

bool Backend::isDebugMode()
{
   return mFi.isDebugMode();
}

bool Backend::getFLSDesk()
{
    return mFLSDeskChecked;
}

bool Backend::getEESM()
{
    return mEESMChecked;
}

void Backend::setFLSDesk(bool bFLSDesk)
{
    mFLSDeskChecked = bFLSDesk;
    qDebug() << "setFLSDesk" << mFLSDeskChecked;
    emit flsDeskChanged();
}

void Backend::setEESCM(bool bEESCM)
{
    mEESMChecked = bEESCM;

    emit eescmChanged();
}


