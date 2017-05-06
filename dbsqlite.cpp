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
    mMissingSwpnsIter = NULL;
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

int DBSqlite::countTable(QString table)
{
    QSqlQuery swpns_cnt(db);
    swpns_cnt.prepare("select count(*) from " + table);
    swpns_cnt.exec();
    swpns_cnt.next();

    //qDebug() << "Count:" << table << swpns_cnt.value(0).toInt();

    int ret = swpns_cnt.value(0).toInt();
    swpns_cnt.finish();
    return ret;
}

void DBSqlite::showCounts()
{
    countTable("vw_pdl_atasection");
    countTable("vw_pdl_atachapter");
    countTable("vw_pdl_airline");
    countTable("vw_pdl_ac");
    countTable("vw_pdl_swpn_ac");
    countTable("vw_pdl_swpn");
    countTable("tmp_swpn_rep");
}

int DBSqlite::cleanDB()
{
    insertRepFound();
    showCounts();
    QSqlQuery swpns_del(db);
    swpns_del.prepare("delete from vw_pdl_swpn where swpn not in (select swpn from tmp_swpn_rep)");
    swpns_del.exec();
    if (swpns_del.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 1"      << se.text();
    }

    QSqlQuery swpns_del_ac(db);
    swpns_del_ac.prepare("delete from vw_pdl_swpn_ac where swpn not in (select swpn from tmp_swpn_rep)");
    swpns_del_ac.exec();
    if (swpns_del_ac.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 2"      << se.text();
    }
    swpns_del_ac.finish();

    QSqlQuery swpns_del_ts(db);
    swpns_del_ts.prepare("delete from vw_pdl_ac where tailsign not in (select distinct tailsign from vw_pdl_swpn_ac)");
    swpns_del_ts.exec();
    if (swpns_del_ts.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 3"      << se.text();
    }
    swpns_del_ts.finish();

    QSqlQuery swpns_del_ia(db);
    swpns_del_ia.prepare("delete from vw_pdl_airline where iata_code not in (select distinct iata_code from vw_pdl_ac)");
    swpns_del_ia.exec();
    if (swpns_del_ia.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 4"      << se.text();
    }
    swpns_del_ia.finish();

    QSqlQuery swpns_del_sec(db);
    swpns_del_sec.prepare("delete from vw_pdl_atasection where acmodel not in (select distinct acmodel from vw_pdl_ac)");
    swpns_del_sec.exec();
    if (swpns_del_sec.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 5"      << se.text();
    }
    swpns_del_sec.finish();

    QSqlQuery swpns_del_cha(db);
    swpns_del_cha.prepare("delete from vw_pdl_atachapter where atachapter_no not in (select distinct atachapter_no from vw_pdl_atasection)");
    swpns_del_cha.exec();
    if (swpns_del_cha.lastError().isValid()) {
        QSqlError se = swpns_del.lastError();
        qDebug() << "Error 6"      << se.text();
    }
    swpns_del_cha.finish();
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

void DBSqlite::insertRepFound() {
    time_t secs = time(0);
    mFoundSwpnsIter = new QMapIterator<QString, QString>(mFoundSwpns);
    QString t;
    QSqlQuery swpns(db);

    if (!mFoundSwpnsIter->hasNext()) {
          delete mFoundSwpnsIter;
          mFoundSwpnsIter = NULL;
          return;
    }

    while (mFoundSwpnsIter->hasNext()) {
        int count = 0;
        QString insertSql = "insert into tmp_swpn_rep ";
        while (mFoundSwpnsIter->hasNext()) {
            mFoundSwpnsIter->next();
            QString swpn    = mFoundSwpnsIter->key();
            int media_count = mFoundSwpnsIter->value().toInt();
            if (count == 0) {
                insertSql      += t.sprintf("      select \"%s\" as 'swpn', %d as 'media_count' ", (const char*)swpn.toUtf8(), media_count);
            } else {
                insertSql      += t.sprintf("union select \"%s\", %d ",                            (const char*)swpn.toUtf8(), media_count);
            }
            if (++count >= 50)
                break;
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
    delete mFoundSwpnsIter;
    mFoundSwpnsIter = NULL;

}

void DBSqlite::resetRepFound() {
    mFoundSwpns.clear();
    QSqlQuery swpns(db);

    swpns.exec("drop table tmp_swpn_rep");
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error 2a"      << se.text();
    }
    swpns.exec("create table tmp_swpn_rep (swpn text, media_count integer)");
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error 2b"      << se.text();
    }

    if (!mpSwpnsRepIns) {
        mpSwpnsRepIns = new  QSqlQuery(db);
        mpSwpnsRepIns->prepare("insert into tmp_swpn_rep values (:swpn, :media_count)");
    }
}

void DBSqlite::addRepInfo(QString swpn, QString media_count, bool inRep) {
    //qDebug() << "addRepInfo" << swpn << media_count.toInt() << inRep;
    if (!inRep) {
        mMissingSwpns.insert(swpn, media_count);
        mMissingCount += media_count.toInt();
        return;
    }
    mFoundSwpns.insert(swpn, media_count);
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

