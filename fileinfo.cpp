
#include "fileinfo.h"
#include <stdlib.h>

QString FileInfo::mTestPath;
FileInfo::FileInfo(QObject *parent) :
    QObject(parent)
{
    if (!mTestPath.isEmpty()) {
        setPath(mTestPath, false);
    } else {
        setPath(MACDIR, false);
        setPath(LINDIR, false);
        setPath(PDLDIR, true);
    }
    QDir t(getTempDir());
    check4TestDir();
    if (!t.exists()) {
        t.mkpath(getTempDir());
    }

}

void FileInfo::check4TestDir() {
    char *dir = getenv("GUI_TESTDIR");
    if (!dir)
        return;
    mPath += '/';
    mPath += dir;
    qDebug() << "check4TestDir" << mPath;
}

bool FileInfo::setPath(const QString &path, bool pdl) {
    QDir p(path);
    if (!p.exists())
        return false;
    //qDebug() << "FileInfo::setPath" << path;
    mPath  = path;
    mOnPDL = pdl;
    return true;
}

int FileInfo::getPort() {
    QFile f(mPath + "/.port");
    int port = 80;
    if (!f.exists())
        return port;

    if (!f.open(QIODevice::ReadOnly))  {
        return port;
    }

    QByteArray line = f.readLine();
    QByteArray v;

    for (int i=0; i < line.size(); i++) {
        if (line[i] >= '0' and line[i] <= '9') {
            v.append(line[i]);
            continue;
        }
        break;
    }
    qDebug() << "v" << v;
    port = v.toInt();

    f.close();

    qDebug() << "PORT" << port << v;
    return port;
}
