#include "Backend.h"
#include "dbsqlite.h"
#include "fileinfo.h"
#include "checkrepository.h"

#include <QDebug>
#include <QProcess>
#include <QFileInfo>

#ifdef Q_WS_WIN
#include <time.h>
#endif

DBSqlite::DBSqlite(QObject *parent) :
    QObject(parent)
{
    mProcess     = NULL;
    mPostProcess = ACT_NULL;
    mpSwpnsRepIns     = NULL;
}

bool DBSqlite::loadNeeded()
{
    if (!QFile::exists(mFi.getDataFileName())) {
        qDebug() << "Failed to find '" << mFi.getDataFileName() << "'";
        return false;
    }
    if (!QFile::exists(mFi.getDataBaseName())) {
        qDebug() << "Failed to find '" << mFi.getDataBaseName() << "'";
        return true;
    }
    QFileInfo lf(mFi.getDataFileName());
    QFileInfo db(mFi.getDataBaseName());

    qDebug() << "loadNeeded:" << db.lastModified() << lf.lastModified();

    return (db.lastModified() < lf.lastModified()) ? true : false;
}

bool DBSqlite::loadDB(bool clean)
{
    FileInfo mFi;

    qDebug() << "loadDb" << clean;

    openDB();

    if (loadNeeded()) {
        QFile f(mFi.getDataFileName());

        qDebug() << "Opening" << mFi.getDataFileName();

        if (!f.open(QIODevice::ReadOnly))  {
            qDebug() << "Failed to open file.";
            return false;
        }

        while (!f.atEnd()) {
            QByteArray line = f.readLine();
            while (line.endsWith('\n') || line.endsWith('\r')) {
                line.remove(line.count() - 1, 1);
            }

            if (line.isEmpty() || line.startsWith("#"))
                continue ;

            //IFDBGSQL() qDebug() << line;
            QSqlQuery sq = db.exec(line);
        }
        f.close();
    }
    mSwpnMetaCount = countTable("vw_pdl_swpn");
    if (clean) {
        verifySWPNs(true);
    } else {
        loadSWPN();
    }
    qDebug() << "loadDB:exiting";
    return true;
}

int DBSqlite::buildDelta()
{
    CheckRepository cr;
    cr.checkAll();
    qDebug() << "Missing count" << mMissingCount;
    return mMissingCount;
}

int DBSqlite::getValidCount()
{
    QSqlQuery swpns_cnt(db);
    QDate now = QDate::currentDate();
    QString sql = QString("select count(*) from vw_pdl_swpn where valid_from <= \"%1\" and valid_until >= \"%1\"")
            .arg(QDate::currentDate().toString("yyyy-MM-dd"));
    qDebug() << "getValidCount:" << sql;
    swpns_cnt.prepare(sql);
    swpns_cnt.exec();
    if (swpns_cnt.lastError().isValid()) {
        QSqlError se = swpns_cnt.lastError();
        qDebug() << "Error 1"      << se.text();
    }
    swpns_cnt.next();

    qDebug() << "Count:" << swpns_cnt.value(0).toInt();

    int ret = swpns_cnt.value(0).toInt();
    swpns_cnt.finish();
    return ret;}

int DBSqlite::countTable(QString table)
{
    int ret = getSingleValue("select count(*) from " + table).toInt();
    qDebug() << "Count:" << table << ret;
    return ret;
}

QString DBSqlite::getSingleValue(QString sql)
{
    QSqlQuery request(db);
    QString ret;
    request.prepare(sql);
    request.exec();
    if (request.lastError().isValid()) {
        QSqlError se = request.lastError();
        qDebug() << "getSingleValue" << sql << se.text();
        return ret;
    }
    request.next();
    ret = request.value(0).toString();
    request.finish();
    //qDebug() << "getSingleValue" << sql << request.value(0).toString();
    return ret;
}

void DBSqlite::showCounts()
{
    qDebug() << "ShowCounts -----------------------";
    countTable("vw_pdl_atasection");
    countTable("vw_pdl_atachapter");
    countTable("vw_pdl_airline");
    countTable("vw_pdl_ac");
    countTable("vw_pdl_swpn_ac");
    countTable("vw_pdl_swpn");
    countTable("tmp_swpn_rep");
}

int DBSqlite::oneSQL(QString sql, QString text)
{
    QSqlQuery onesql(db);
    onesql.prepare(sql);
    onesql.exec();
    if (onesql.lastError().isValid()) {
        QSqlError se = onesql.lastError();
        qDebug() << text << se.text();
    }
    onesql.finish();
}

int DBSqlite::cleanDB()
{
    insertRepFound(mFoundSwpns,   REP_SYNCED);
    insertRepFound(mMissingSwpns, REP_NOTSYNCED);
    insertRepFound(mNotRefSwpns,  REP_NOTREF);
    showCounts();

    oneSQL("delete from vw_pdl_swpn where swpn not in (select swpn from tmp_swpn_rep where status = 1)",                      "delete swpn");
    oneSQL("delete from vw_pdl_swpn_ac where swpn not in (select swpn from tmp_swpn_rep where status = 1)",                   "delete swpn_ac");
    oneSQL("delete from tmp_swpn_rep where swpn in (select swpn from vw_pdl_swpn) and status = 1",                            "delete repository swpn");
    oneSQL("delete from vw_pdl_ac where tailsign not in (select distinct tailsign from vw_pdl_swpn_ac)",                      "delete ac");
    oneSQL("delete from vw_pdl_airline where iata_code not in (select distinct iata_code from vw_pdl_ac)",                    "delete airline");
    oneSQL("delete from vw_pdl_atasection where acmodel not in (select distinct acmodel from vw_pdl_ac)",                     "delete section");
    oneSQL("delete from vw_pdl_atachapter where atachapter_no not in (select distinct atachapter_no from vw_pdl_atasection)", "delete chapter");
    showCounts();

    return 0;
}

int DBSqlite::verifySWPNs(bool remove)
{
    if (gBackend->mFi.isVerified())
            return 0;

    int count = buildDelta();
    qDebug() << "Download count:" << count << remove;

    if (remove) {
        cleanDB();
        loadSWPN();
    }
    showCounts();
    gBackend->mFi.setVerified(true);
    return count;
}

void DBSqlite::loadSWPN(bool extras) {
    mSwpnList.clear();
    mSwpnsMC.clear();
    if (extras) {
        mSwpnList.append("SIMULATE_1");
        mSwpnList.append("SIMULATE_2");
        mSwpnList.append("EMULATE_1");
    }

    QSqlQuery swpns(db);
    swpns.exec("select swpn, media_count from vw_pdl_swpn");

    if (swpns.lastError().isValid()) {
        qDebug() << "loadSWPN Error" << swpns.lastError().text();
        return;
    }
    while (swpns.next()) {
        //qDebug() << "Valid swpn" << swpns.value(0).toString();
        mSwpnList.append(swpns.value(0).toString());
        mSwpnsMC.insert(swpns.value(0).toString(), swpns.value(0).toString());
    }
    swpns.finish();
}

void DBSqlite::resetRepInfo() {
    mMissingSwpns.clear();
    mMissingCount = 0;
    resetRepFound();
}

void DBSqlite::insertRepFound(QMap<QString, QString> &map, repstat_t repstat) {
    time_t secs = time(0);
    QMapIterator<QString, QString> mapIter(map);
    QString t;
    QSqlQuery swpns(db);

    QString format0(" select \"%1\" as 'swpn', %2 as 'status'");
    QString formatN(" union select \"%1\", %2");
    while (mapIter.hasNext()) {
        int count = 0;
        QString insertSql = "insert into tmp_swpn_rep";
        while (mapIter.hasNext() and count++ < 50) {
            mapIter.next();
            QString swpn    = mapIter.key();
            //qDebug() << "Repfound:" << swpn;
            insertSql += ((count == 1) ? format0 : formatN).arg(swpn).arg(QString::number(repstat));
        }
        insertSql += ";";

        //qDebug() << "insertRepFound:" << insertSql;

        swpns.exec(insertSql);
        if (swpns.lastError().isValid()) {
            QSqlError se = swpns.lastError();
            qDebug() << "Error 2a"      << se.text();
        }
        // qDebug() << "insertRepFound:" << time(0) << count;
    }
    qDebug() << "insertRepFound" << "Elapsed:" << time(0) - secs;
}

void DBSqlite::resetRepFound() {
    mFoundSwpns.clear();
    mMissingSwpns.clear();
    mNotRefSwpns.clear();

    oneSQL("drop table tmp_swpn_rep",                                    "Drop tmp repository table");
    oneSQL("create table tmp_swpn_rep (swpn text, status integer)", "create tmp repository table");
    oneSQL("create unique index unq_s_tmpswpn on tmp_swpn_rep ( swpn )", "create index tmp repository table");
}

void DBSqlite::addRepInfo(QString swpn, QString media_count, int status) {
    //qDebug() << "addRepInfo" << swpn << media_count.toInt() << status;
    switch(status) {
    case REP_SYNCED:
        mFoundSwpns.insert(swpn, media_count);
        break;
    case REP_NOTSYNCED:
        mMissingSwpns.insert(swpn, media_count);
        mMissingCount += media_count.toInt();
        break;
    case REP_NOTREF:
        mNotRefSwpns.insert(swpn, media_count);
        break;
    }
}

void DBSqlite::openNewSwpn() {
    if (mMissingSwpnsIter) {
        delete mMissingSwpnsIter;
    }
    mMissingSwpnsIter = new QMapIterator<QString, QString>(mMissingSwpns);
}

bool DBSqlite::nextNewSwpn(int *count, QString &swpn) {
    qDebug() << "nextNewSwpn:Enter";
    if (!mMissingSwpnsIter) {
        qDebug() << "No list";
        return false;
    }
    if (!mMissingSwpnsIter->hasNext()) {
        qDebug() << "None left";
        delete mMissingSwpnsIter;
        mMissingSwpnsIter = NULL;
        return false;
    }

    mMissingSwpnsIter->next();
    swpn    = mMissingSwpnsIter->key();
    *count  = mMissingSwpnsIter->value().toInt();
    qDebug() << "nextNewSwpn" << swpn << *count;
    return true;
}

void DBSqlite::endNewSwpn() {
    if (!mMissingSwpnsIter)
        return;
    delete mMissingSwpnsIter;
    mMissingSwpnsIter = NULL;
}

QStringList DBSqlite::getRepositoryData()
{
    QSqlQuery query;
    if (query.exec("SELECT * FROM vw_pdl_swpn order by swpn")) {
        QStringList columns;

        QStringList column;
        while (query.next()) {
            column.clear();
            column.append(query.value(0).toString());

            int seconds = query.value(1).toInt();
            int minutes = seconds / 60;
            seconds %= 60;
            QString minStr = (minutes < 10) ? "0" + QString::number(minutes) : QString::number(minutes);
            QString secStr = (seconds < 10) ? "0" + QString::number(seconds) : QString::number(seconds);

            column.append(query.value(2).toString());
            column.append(QString("%1:%2").arg(minStr).arg(secStr));
            column.append(query.value(3).toString());

            columns.append(column.join(";"));
        }

        return columns;
    }

    return QStringList();
}

bool DBSqlite::checkSWPN(QString swpn, int count)
{
    FileInfo mFi;
    QString proc = mFi.getScriptName() + "-k -s " + swpn;

    int ret = QProcess::execute(proc);

    qDebug() << "checkSWPN:" << swpn << count << ret;
    return (ret) ? true : false;
}

void DBSqlite::getSWPNs()
{

}

bool DBSqlite::loadSqlFile(QString sqlfile)
{
    FileInfo mFi;

    if (!QFile::exists(sqlfile)) {
        qDebug() << "Failed to find '" << sqlfile << "'";
        return false;
    }

    QFile f(sqlfile);

    qDebug() << "Opening" << sqlfile;

    if (!f.open(QIODevice::ReadOnly))  {
        qDebug() << "Failed to open file.";
        return false;
    }

    while (!f.atEnd()) {
        QByteArray line = f.readLine();
        while (line.endsWith('\n') || line.endsWith('\r')) {
            line.remove(line.count() - 1, 1);
        }

        if (line.isEmpty() || line.startsWith("#"))
            continue ;

        //IFDBGSQL qDebug() << line;
        QSqlQuery sq = db.exec(line);
        if (sq.lastError().isValid()) {
            qDebug() << "loadSWPN Error" << sq.lastError().text();
        }
    }
    f.close();
    return true;
}

bool DBSqlite::closeDB()
{
    if (db.isOpen()) {
        db.close();
    }
    return true;
}

bool DBSqlite::openDB()
{
    // Find QSLite drive
    if (db.isOpen())
        return true;

    qDebug() << "DBSqlite::openDB";

    db = QSqlDatabase::addDatabase("QSQLITE");

    FileInfo mFi;
    db.setDatabaseName(mFi.getDataBaseName());

    // Open databasee
    return db.open();
}

QSqlError DBSqlite::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DBSqlite::deleteDB()
{
    // Close database
    return closeDB();

}

void DBSqlite::finishedApp()
{
    qDebug() << "finishedApp" << mPostProcess;
    switch(mPostProcess) {
    case ACT_NULL:
        break;
    case ACT_GETSWPNS:
        countTable("tmp_swpn_rep");
        loadSqlFile(mFi.getValidSwpns());
        countTable("tmp_swpn_rep");
        break;
    }
}

void DBSqlite::actionGetSwns()
{
    countTable("tmp_swpn_rep");
    loadSqlFile(mFi.getValidSwpns());
    countTable("tmp_swpn_rep");
}

void DBSqlite::runCommand(QString command)
{
    /* Make sure that the old process will be deleted later */
    if (mProcess)
        mProcess->deleteLater();

    mProcess = new QProcess();

    /* Connect to readyReadStandardOutput to get the output */
    // connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdOut()));
    /* Connect to the finished(int) signal to get when the application is done */
    connect(mProcess, SIGNAL(finished(int)),             this, SLOT(finishedApp()), Qt::QueuedConnection);
    mProcess->start(command);
}

QString DBSqlite::getEstimateLoadTime(QString swpn)
{
    QSqlQuery swpns(db);
    swpns.prepare("select dl_time from vw_pdl_swpn where swpn = :swpn");
    swpns.bindValue(":swpn", swpn);
    swpns.exec();
    swpns.next();
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error 4"      << se.text();
    }
//    qDebug() << "EstimateLoadTime:" << swpns.value(0).toString();

    QString t;
    int secs = swpns.value(0).toInt();
    secs = 326;
    if (secs) {
        int mins = ((secs % 3600) / 60);
        int hrs  = ((secs / 3600));
        secs    %= 60;
        t = t.sprintf("%02d:%02d:%02d", hrs, mins, secs);
    }
    return t;
}

QString DBSqlite::getMediaCount(QString swpn)
{
    QSqlQuery swpns(db);
    swpns.prepare("select media_count from vw_pdl_swpn where swpn = :swpn");
    swpns.bindValue(":swpn", swpn);
    swpns.exec();
    swpns.next();
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error"      << se.text() << swpn;
        qDebug() << "swpns.size" << swpns.size();
    }
    //qDebug() << swpn << "media count:" << swpns.value(0).toString();
    QString ret = swpns.value(0).toString();
    swpns.finish();
    return ret;
}

QString DBSqlite::getSoftwareTitle(QString swpn)
{
    QSqlQuery swpns(db);
    swpns.prepare("select title from vw_pdl_swpn where swpn = :swpn");
    swpns.bindValue(":swpn", swpn);
    swpns.exec();
    swpns.next();
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error"      << se.text();
        qDebug() << "swpns.size" << swpns.size();
    }
    qDebug() << "softwaretitle:" << swpns.value(0).toString();
    return swpns.value(0).toString();
}

QString DBSqlite::getCocFormIssuer(QString swpn)
{
    QSqlQuery swpns(db);
    swpns.prepare("select coc_form1_issuer from vw_pdl_swpn where swpn = :swpn");
    swpns.bindValue(":swpn", swpn);
    swpns.exec();
    swpns.next();
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error"      << se.text();
        qDebug() << "swpns.size" << swpns.size();
    }
    //qDebug() << "CocFormIssuer:" << swpns.value(0).toString();
    return swpns.value(0).toString();
}

QString DBSqlite::getLoadingStandard(QString swpn)
{
    QSqlQuery swpns(db);
    swpns.prepare("select loading_standard from vw_pdl_swpn where swpn = :swpn");
    swpns.bindValue(":swpn", swpn);
    swpns.exec();
    swpns.next();
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error"      << se.text();
        qDebug() << "swpns.size" << swpns.size();
    }
    //qDebug() << "CocFormIssuer:" << swpns.value(0).toString();
    return swpns.value(0).toString();
}

