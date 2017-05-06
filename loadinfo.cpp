#include "loadinfo.h"
#include "fileinfo.h"
#include "Backend.h"
#include <QDebug>

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

LoadInfo::LoadInfo(QObject *parent) :
    QObject(parent)
{
    mCurr = -1;
    FileInfo fi;
    mLoaddir.setPath(fi.getLoadsDir());
    mLoaddir.setFilter(QDir::AllDirs|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    mLoaddir.refresh();
    qDebug() << "loadinfo:" + fi.getLoadsDir();

    QFileInfoList files = mLoaddir.entryInfoList();
    for (int i=0; i < files.count(); i++) {
        QFileInfo qfi = files[i];
        QString fn = qfi.absoluteFilePath();
        QFile fnu(fn + "/.uploaded");
        qDebug() << "loadinfo dir:" + fn;
        if (fnu.exists())
            continue;
        QFile fni(fn + "/.loadinfo");
        qDebug() << "loadinfo nou:" + fn;
        if (!fni.exists())
            continue;
        if (fni.size() == 0) {
            removeDir(fn);
            continue;
        }
        qDebug() << "loadinfo app:" + fn;
        mDirs.append(fn);
        //gBackend->addLog("Adding:" + qfi.fileName());
    }
}

bool LoadInfo::removeDir(const QString & dirName)
{
    QDir dir(dirName);

    if (!dir.exists(dirName))
        return true;

    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (info.isDir()) {
            if (!removeDir(info.absoluteFilePath()))
                return false;
            continue;
        }
        if (!QFile::remove(info.absoluteFilePath()))
            return false;
    }
    return dir.rmdir(dirName);
}

// Loadinfo contains a single line with the following | separated fields

bool LoadInfo::getNextLoad() {
    mZips.empty();

    if (++mCurr >= mDirs.count()) {
        qDebug() << "lastfile:";
        return false;
    }

    QString fn = mDirs[mCurr];
    if (!getLoadInfo(fn + "/.loadinfo")) {
        qDebug() << "no .loadinfo" + fn;
        return false;
    }

    QString t = fn;
    //t.remove(0,76);
    //gBackend->addLog(t);
    getFiles2Upload(fn);

    return true;
}

QString LoadInfo::getCurrDir() {
    QFileInfo qfi = mDirs[mCurr];
    return qfi.baseName();
}

void LoadInfo::getFiles2Upload(QString dir) {
    QDir loaddir;
    loaddir.setPath(dir);
    loaddir.setFilter(QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
    loaddir.refresh();
    mZips.empty();
    qDebug() << "getFiles2Upload:" + dir;

    QFileInfoList files = loaddir.entryInfoList();
    for (int i=0; i < files.count(); i++) {
        QFileInfo qfi = files[i];
        QString fn = qfi.fileName();
        qDebug() << "filename:" + fn;
        if (!(fn.endsWith(".zip", Qt::CaseInsensitive) || fn.endsWith(".tar", Qt::CaseInsensitive)))
            continue;
        qDebug() << "basename:" + qfi.baseName();
        mZips.append(qfi.baseName());
    }
}

void LoadInfo::appendFiles2Message(QByteArray &mess, QString &boundary) {
    QString dir = mDirs[mCurr];
    qDebug() << "appendFiles2Message:" + dir;
    char buf[1024];
    QString t;

    for (int i=0; i < mFileinfo.count(); i++) {
        QStringList qfi = mFileinfo[i].split('|');
        QString fileName = dir + "/" + qfi[4];
        qDebug() << "1 appendFiles2Message" << fileName << mFileinfo[i];
        if (!QFile::exists(fileName))
            continue;

        QString tname = t.sprintf("apache-servicemix-4.4.%d.zip", i);
        SNPRINTF(buf, 1024,
                "Content-Id: <%s>\r\n"
                "Content-Type: application/octet-stream; name=%s\r\n"
                "Content-Transfer-Encoding: binary\r\n"
                "Content-Disposition: attachment; name=\"%s\"; filename=\"%s\"\r\n\r\n",
                 (const char*)qfi[3].toUtf8(),
                 (const char*)tname.toUtf8(),
                 (const char*)tname.toUtf8(),
                 (const char*)tname.toUtf8());
        mess += buf;
        qDebug() << "2 appendFiles2Message" << fileName;
        QFile f(fileName);
        if (!f.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file.";
            continue;
        }
        mess += f.readAll();
        f.close();

        mess += "\r\n" + boundary;
    }
}

void LoadInfo::setUploaded() {

    if (mCurr < 0 || mCurr >= mDirs.count())
        return;

    removeDir(mDirs[mCurr]);
    return;

    QString fileName = mDirs[mCurr] + "/.uploaded";
    if (QFile::exists(fileName))
        return;

    QFile f(fileName);

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file:" + fileName;
        return;
    }
    f.close();
}

void LoadInfo::reset() {
}

bool LoadInfo::getLoadInfo(QString fileName) {
    mLoadinfo.clear();
    mFileinfo.clear();

    if (!QFile::exists(fileName))
        return false;

    QFile f(fileName);

    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file.";
        return false;
    }
    while (!f.atEnd()) {
        QString line = f.readLine();
        while (line.endsWith('\n') || line.endsWith('\r')) // Trim CR/LF
            line.remove(line.count() - 1, 1);

        if (line.isEmpty() || line.startsWith("#"))
            continue;

        if (line.startsWith("INFO")) {
            mLoadinfo = line.split('|');
            mLoadinfo.pop_front();
            continue;
        }
        if (line.startsWith("FILE")) {
            mFileinfo += line;
            continue;
        }
    }
    return (mLoadinfo.count() == 0) ? false : true;
}
