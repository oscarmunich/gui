#include "dbsettings.h"
#include "Backend.h"
#include "fileinfo.h"
#include <QDebug>
#include <QTime>
#include <QProcess>

DBSettings::DBSettings(QObject *parent) :
    QObject(parent)
{
}

bool DBSettings::loadDB()
{
    //db = QSqlDatabase::addDatabase("QSQLITE", "Settings");

    //FileInfo mFi;
    //db.setDatabaseName(mFi.getSettingsDataBaseName());

    //db.open();

    openDB();

    checkForLocal();
    loadDBFromFile();
    hashDB();
    setTime();
    load_pdl_name();
    //db.close();

    //QSqlDatabase::removeDatabase("Settings");

    return true;
}

void DBSettings::setTime()
{
    time_t time = QDateTime::currentDateTimeUtc().toTime_t();
    time_t servTime;
    QString servTimeDate;
    if (getValueInt("TimeSec", (int *)&servTime)) {
        if (servTime > time) {
            getDBValue("Time", servTimeDate);
            QString t = "date " + servTimeDate;
            qDebug() << "Setdate:" << t;
            QProcess::execute(t);
        }
    }

}

void DBSettings::checkForLocal()
{
    FileInfo mFi;

    QSqlQuery swpns(db);
    swpns.exec("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='local_pdlsetting'");
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error a" << se.text();
    }
    swpns.next();

    if (swpns.value(0).toInt() == 0) {
        qDebug() << "Creating local settings table";
        swpns.exec("CREATE TABLE local_pdlsetting ( setkey, setvalue )");
        if (swpns.lastError().isValid()) {
            QSqlError se = swpns.lastError();
            qDebug() << "Error b" << se.text();
        }
        QByteArray v;
        qDebug() << "load_update_timestamp" << v;
        getFile(mFi.getSyncTimeFile(), v);
        swpns.exec("INSERT INTO  local_pdlsetting VALUES ('update_timestamp', v)");
        if (swpns.lastError().isValid()) {
            QSqlError se = swpns.lastError();
            qDebug() << "Error c" << se.text();
        }
    }
    swpns.finish();
}

bool DBSettings::loadDBFromFile()
{
    FileInfo mFi;

    if (!QFile::exists(mFi.getSettingsFileName())) {
        qDebug() << "Failed to find '" << mFi.getSettingsFileName() << "'";
        return false;
    }

    QFile f(mFi.getSettingsFileName());
    qDebug() << "Opening" << mFi.getSettingsFileName();

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

        //qDebug() << line;
        QSqlQuery sq = db.exec(line);
    }
    return true;
}

bool DBSettings::closeDB()
{
    if (db.isOpen()) {
        db.close();
    }
    return true;
}

void DBSettings::hashDB()
{
    mdbinfo.clear();
    QSqlQuery swpns(db);
    swpns.exec("select * from vw_pdl_pdlsetting");
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error d" << se.text();
    }
    while (swpns.next()) {
        QString key    = swpns.value(0).toString();
        QString value  = swpns.value(1).toString();
        //qDebug() << key << value;
        mdbinfo[key] = value;
    }
    QSqlQuery swpnsl(db);
    swpnsl.exec("select * from local_pdlsetting");
    if (swpnsl.lastError().isValid()) {
        QSqlError se = swpnsl.lastError();
        qDebug() << "Error e" << se.text();
    }
    while (swpnsl.next()) {
        QString key    = swpnsl.value(0).toString();
        QString value  = swpnsl.value(1).toString();
        //qDebug() << key << value;
        mdbinfo[key] = value;
    }
}

void DBSettings::dumpDB()
{
    QSqlQuery swpns(db);
    swpns.exec("select * from vw_pdl_pdlsetting");
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error f" << se.text();
    }
    qDebug() << "swpns.size" << swpns.size();
    while (swpns.next()) {
        QString key    = swpns.value(0).toString();
        QString value  = swpns.value(1).toString();
        //qDebug() << key << value;
    }
}

bool DBSettings::setValueInt(const QString &key, int value) {
    QSqlQuery swpns(db);
    QString t;
    QString tval = t.sprintf("%d", value);

    swpns.exec(t.sprintf("UPDATE local_pdlsetting SET setvalue = '%s' WHERE setkey = '%s'",
            (const char *)tval.toUtf8(),
            (const char *)key.toUtf8()));

    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error g" << se.text();
    }
    mdbinfo[key] = tval;
    return true;
}

bool DBSettings::insertKey(const QString &key) {
    QSqlQuery swpns(db);
    QString t;
    QString com = t.sprintf("SELECT count(*) FROM local_pdlsetting WHERE setkey= '%s'",
                            (const char *)key.toUtf8());

    swpns.exec(com);
    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error h" << se.text();
    }
    swpns.next();
    qDebug() << "insertKey" << swpns.value(0) << swpns.value(0).toInt() << com;

    if (swpns.value(0).toInt() == 0) {
        swpns.exec(t.sprintf("INSERT INTO local_pdlsetting VALUES('%s', '')",
                (const char *)key.toUtf8()));
        if (swpns.lastError().isValid()) {
            QSqlError se = swpns.lastError();
            qDebug() << "Error i" << se.text();
        }
    }
    return true;
}

bool DBSettings::setValue(const QString &key, const QString &val) {
    QSqlQuery swpns(db);
    QString t;

    insertKey(key);
    swpns.exec(t.sprintf("UPDATE local_pdlsetting SET setvalue = '%s' WHERE setkey = '%s'",
            (const char *)val.toUtf8(),
            (const char *)key.toUtf8()));

    if (swpns.lastError().isValid()) {
        QSqlError se = swpns.lastError();
        qDebug() << "Error j" << se.text();
    }
    mdbinfo[key] = val;
    return true;
}


bool DBSettings::getDBValue(const QString &key, QString &value) {
    if (mdbinfo.contains(key)) {
        value = mdbinfo[key];
        return true;
    }
    return false;
}

bool DBSettings::openDB()
{
    // Find QSLite drive
    qDebug() << "DBSettings::openDB";
    if (db.isOpen())
        return true;

    db = QSqlDatabase::addDatabase("QSQLITE", "Settings");

    FileInfo mFi;
    db.setDatabaseName(mFi.getSettingsDataBaseName());

    // Open databasee
    return db.open();
}

QSqlError DBSettings::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

bool DBSettings::deleteDB()
{
    // Close database
    return closeDB();
}

bool DBSettings::getFile(QString file, QByteArray & value) {
    QFile f(file);
    qDebug() << "Opening" << file;

    if (!f.open(QIODevice::ReadOnly))  {
        qDebug() << "Failed to open file." << file;
        return false;
    }
    value = f.read(64);
    qDebug() << "getFile" << file << value;
    f.close();
    return true;
}

void DBSettings::write2File(QString file, QString value) {
    QFile f(file);
    qDebug() << "Opening" << file;

    if (!f.open(QIODevice::WriteOnly))  {
        qDebug() << "Failed to open file.";
        return;
    }
    f.write(value.toUtf8());
    f.close();
}

void DBSettings::load_update_timestamp() {
    FileInfo mFi;
    QByteArray v;
    getFile(mFi.getSyncTimeFile(), v);
    qDebug() << "load_update_timestamp" << v;
    setValueInt("update_timestamp", v.toInt());
}

void DBSettings::load_pdl_name() {

    if (!(mdbinfo["pdl_name"].isNull() or mdbinfo["pdl_name"].isEmpty()))
        return;

    FileInfo mFi;
    QByteArray v;
    qDebug() << "load_pdl_name" << v;
    getFile("/etc/hostname", v);
    QString value;
    for (int i=0; i < v.count(); i++)
        value.append((const char)v[i]);
    mdbinfo["pdl_name"] = value;
}

bool DBSettings::set_update_timestamp(int value) {
    FileInfo mFi;
    QString v;
    v = v.sprintf("%d", value);
    qDebug() << "set_update_timestamp" << value << v;
    write2File(mFi.getSyncTimeFile(), v);
    return setValueInt("update_timestamp", value);
}

bool DBSettings::set_db_time(QString value) {
    FileInfo mFi;
    qDebug() << "set_db_time" << value;
    return setValue("db_time", value);
}

bool DBSettings::set_db_pdl_change_time(QString value) {
    FileInfo mFi;
    qDebug() << "set_pdl_change_time" << value;
    return setValue("db_pdl_change_time", value);
}

bool DBSettings::get_pdl_name(QString &value) {
    return getValueString("pdl_name", value);
}
