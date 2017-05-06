#include "checkrepository.h"
#include "Backend.h"
#include "fileinfo.h"
#include "dbsqlite.h"
#include <QCryptographicHash>
#include "checkrepository.h"

#ifdef Q_WS_WIN
#include <time.h>
#endif

QString CheckRepository::mVerified(".verified");
QString CheckRepository::mInfo(".info");

CheckRepository *gCheckRep;

void CheckRepository::createCheckThread()
{
    if (gCheckRep)
        return;

    gCheckRep                  = new CheckRepository();
    gCheckRep->mCheckThread    = new SyncThread;
    gCheckRep->moveToThread(gCheckRep->mCheckThread);
    gCheckRep->mCheckThread->start();
}

CheckRepository::CheckRepository(QObject *parent) :
    QObject(parent)
{
    mRepPath = gBackend->mFi.getRepositorySWDir();
    QString t;
    for (int i=0; i <= 25; i++) {
        mDotDisk.append(t.sprintf(".DISK#%02d", i));
        mZipDisk.append(t.sprintf("DISK#%02d.zip", i));
    }
}

bool CheckRepository::checkAll(bool force) {
    time_t secs = time(0);
    gBackend->getDB().resetRepInfo();
    gBackend->getDB().loadSWPN(false);
    QStringList swpns = gBackend->getDB().getSwpnList();
    QStringListIterator li(swpns);
    while (li.hasNext()) {
        QString pn = li.next();
        bool valid = checkPartNumber(pn, force);
        if (!valid)
            qDebug() << "checkPartNumber" << pn << valid;
        gBackend->getDB().addRepInfo(pn, mMediaCount, valid);
    }
    qDebug() << "checkAll" << force << "Elapsed:" << time(0) - secs;

    return true;
}

void CheckRepository::checkPN(QString partnumber)
{
    qDebug() << "CheckRepository::checkPN" << partnumber;
    bool res = checkPartNumber(partnumber);
    qDebug() << "CheckRepository::checkPN" << res;
    emit PNOk(partnumber, res);
}

bool CheckRepository::checkPartNumber(QString partnumber, bool force)
{
    QString t;
    QString di;
    QString df;

    mPartNumber    = partnumber;
    mPartNumberDir = mRepPath + "/" + pn2fs(mPartNumber);

    mMediaCount      = gBackend->getDB().getMediaCount(mPartNumber);
    int mediacount   = mMediaCount.toInt();

    QDir d(mPartNumberDir);
    if (!d.exists()) {
        return false;
    }

    if (mediacount == 0)
        return false;

    if (fileExists(mVerified)) {
        if (!force)
            return true;
        removeFile(mVerified);
    }
    //qDebug() << "mPartNumberDir" << mVerified;
    if (!fileExists(mInfo))
        return false;

    for (int i=1; i <= mediacount; i++) {
        di = mDotDisk.at(i);
        df = mZipDisk.at(i);
        if (!(fileExists(di) && fileExists(df))) {
                return false;
        }
        if (!getAttributes(di)) {
            return false;
        }
        if (!checkSize(df)) {
            return false;
        }
        if (!checkChecksum(df)) {
            return false;
        }
    }
    createFile(mVerified);
    return true;
}

bool CheckRepository::checkPartNumber4Load(QString partnumber)
{
    if (!doPartNumberCheck(partnumber)) {
        removeFile(mVerified);
        return false;
    }
    return true;
}

bool CheckRepository::doPartNumberCheck(QString partnumber)
{
    QString t;
    QString di;
    QString df;

    mPartNumber    = partnumber;
    mPartNumberDir = mRepPath + "/" + pn2fs(mPartNumber);

    mMediaCount      = gBackend->getDB().getMediaCount(mPartNumber);
    int mediacount   = mMediaCount.toInt();

    QDir d(mPartNumberDir);
    if (!d.exists()) {
        return false;
    }

    if (mediacount == 0)
        return false;

    if (!fileExists(mInfo))
        return false;

    for (int i=1; i <= mediacount; i++) {
        di = mDotDisk.at(i);
        df = mZipDisk.at(i);
        if (!(fileExists(di) && fileExists(df))) {
            return false;
        }
        if (!getAttributes(di)) {
            return false;
        }
        if (!checkSize(df)) {
            return false;
        }
        if (!checkChecksum(df)) {
            return false;
        }
    }
    return true;
}

bool CheckRepository::checkSize(const QString & file) {
    QFile f(getFilePath(file));
    if (f.size() != mFileSize) {
        return false;
    }
    return true;
}

#define BUFFERSIZE 65536

bool CheckRepository::checkChecksum(const QString & file) {
    QCryptographicHash sha(QCryptographicHash::Sha1);
    char data[BUFFERSIZE];
    QFile f(getFilePath(file));

    if (!f.open(QIODevice::ReadOnly))  {
        qDebug() << "Failed to open file." << file;
        return false;
    }

    while (!f.atEnd()) {
        int length = f.read(data, BUFFERSIZE);
        sha.addData(data, length);
    }
    f.close();

    QByteArray result = sha.result().toHex();
    qDebug() << "checkChecksum" << result << mChecksum;
    if (result != mChecksum)
        return false;
    return true;
}

bool CheckRepository::getAttributes(const QString & file) {
    QFile f(getFilePath(file));
    QList <QByteArray> list;

    if (!f.open(QIODevice::ReadOnly))  {
        qDebug() << "Failed to open file." << file;
        return false;
    }

    while (!f.atEnd()) {
        QByteArray line = f.readLine();
        while (line.endsWith('\n') || line.endsWith('\r')) {
            line.remove(line.count() - 1, 1);
        }
        if (line.isEmpty() || line.startsWith("#"))
            continue ;
        list = line.split('|');

        if (list.count() != 2)
            return false;

        if (list[0] == "media.filesize") {
            mFileSize = list[1].toUInt();
            //qDebug() << "media.filesize" << mFileSize;
            continue;
        }
        if (list[0] == "media.checksum") {
            mChecksum = list[1];
            //qDebug() << "media.checksum" << mChecksum;
            continue;
        }
    }
    f.close();
    return true;
}

QString CheckRepository::getFilePath(const QString & file) {
    QString fp =  mPartNumberDir + "/" + file;
    //qDebug() << "getFilePath:" << fp;
    return fp;
}
bool CheckRepository::fileExists(QString file) {
    QFile f(getFilePath(file));
    return f.exists();
}

bool CheckRepository::removeFile(QString file) {
    QFile f(getFilePath(file));
    f.remove();
    return true;
}

bool CheckRepository::createFile(QString file) {
    QFile f(getFilePath(file));
    if (!f.open(QIODevice::WriteOnly))  {
        qDebug() << "Failed to open file." << file;
        return false;
    }
    f.close();
    return true;
}

QString CheckRepository::pn2fs(QString pn) {
    pn.replace('/', '%');
    pn.replace(' ', '@');
    return pn;
}

QString CheckRepository::fs2pn(QString pn) {
    pn.replace('%', '/');
    pn.replace('@', ' ');
    return pn;
}
