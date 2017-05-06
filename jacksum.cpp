#include "jacksum.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

Jacksum::Jacksum(QObject *parent) : QObject(parent)
{

}

QString
Jacksum::getDiskJacksumInfo(const QString &filepath){

    QString retval = "";
    QFile f(filepath);
    if (!f.open(QFile::ReadOnly | QFile::Text)){
        //LOG_ERROR("Failed to open file: %s", qPrintable(filepath));
        return retval;
    }
    QTextStream in(&f);
    while (!in.atEnd()){
        QString line = in.readLine();
        if (!line.startsWith("media.mediaJacksum")){
            continue;
        }
        retval = line.mid(line.indexOf("|")+1);
        break;
    }
    f.close();
    return retval;
}

bool
Jacksum::checkAllSWPNJackums(){
    QString mRepoPath = "/Users/kevin/PDL_dev/PDL_home/home/root/loader/Repository/SW";
    QString mSwpnDirName = "2366-HAX-351-01";
    QString mLoaderType;
    QString mOutDirPath = "/Users/kevin/PDL_dev/PDL_home/home/root/loader/Repository/EX";
    QString path = mRepoPath + "/" + mSwpnDirName;
    QDir swpndir(mOutDirPath);
    printf ("checkAllSWPNJackums\n");
    QStringList filelist = swpndir.entryList(QStringList("[A-Z0-9]*"), QDir::Dirs | QDir::NoSymLinks);
    foreach (const QString &item, filelist) {
        qDebug() << item;
        checkSWPNJackums(item);
        //break;
    }
    return true;
}

bool
Jacksum::checkSWPNJackums(const QString &mSwpn){
    QString mRepoPath = "/Users/kevin/PDL_dev/PDL_home/home/root/loader/Repository/SW";
    QString mSwpnDirName = mSwpn;
    QString mLoaderType;
    QString mOutDirPath = "/Users/kevin/PDL_dev/PDL_home/home/root/loader/Repository/EX";
    QString path = mRepoPath + "/" + mSwpnDirName;
    QDir swpndir(path);
    QStringList filelist = swpndir.entryList(QStringList(".DISK#*"),
                                QDir::Files | QDir::NoSymLinks | QDir::Hidden);
    //qDebug() << "path" << path;
    //qDebug() << filelist;

    // on Secureusb when we have one disk, just put the files into the root
    if ((filelist.size() == 1) && (mLoaderType == "SecureUSB")){
            QString jackold = getDiskJacksumInfo(mRepoPath + "/" + mSwpnDirName
            + "/.DISK#01");
            QString extractdir = QDir::cleanPath(mOutDirPath) + "/";
            //LOG_DEBUG("check dir : %s", qPrintable(extractdir));
            //LOG_DEBUG("old jack: %s", qPrintable(jackold));
            QString jacknew = calculateDirJacksum(extractdir);
            if (jacknew != jackold.trimmed()){
                //addLog("Jacksum of LSAP: "+ mSwpn + " is invalid!");
                return false;
            }
            return true;
    }

    foreach (const QString &item, filelist) {
        QString jackold = getDiskJacksumInfo(mRepoPath + "/" + mSwpnDirName + "/" + item);
        QString disk = mSwpnDirName + "/" + item;
            QString extractdir = mOutDirPath + "/" + mSwpnDirName + "/" + item.mid(1);
            //LOG_DEBUG("check dir : %s", qPrintable(extractdir));
            //LOG_DEBUG("old jack: %s", qPrintable(jackold));

            QString jacknew = calculateDirJacksum(extractdir);
            QString res = (jacknew == jackold.trimmed()) ? "OK" : "NOK";
            //if (jacknew != jackold.trimmed()){
              //  addLog("Jacksum of LSAP: "+ mSwpn + " media: " +
              //              item.mid(1) + " is invalid!");
                //return false;
            //}
            qDebug() << disk << jackold << jacknew << res;
    }
    return true;
}

/* basically emulates
 * jacksum -a sha1 -S -r -f -w /path/to/dir
 * by traversing the dir rescursively, calculates/accumulates the sha1 hashes
 * of each files and their relative path (the path relative to the given root
 * dirpath and converted to Latin1 (ISO-5589-1).
 * This method blocks, so shouldn't be called from the main GUI thread but
 * either call it from a working thread or wrap it with moveToThread()
 * see http://www.jonelo.de/java/jacksum/
*/
QString
Jacksum::calculateDirJacksum(const QString& dirpath){

    //LOG_DEBUG("checking dir: %s",qPrintable(dirpath));
    QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Sha1);
    traverseDirForJacksum(dirpath, hash, dirpath);
    QByteArray result = hash->result().toHex();
    //LOG_INFO("dir jacksum result: %s", qPrintable(result));
    delete hash;
    return result;
}


/* the workhorse under the calculateDirJacksum(), it traverses into the given
 * dir recursively and add the file's content's and the file relative path'a
 * sha1 checksum to the hash QCryptographicHash. In order to calculate the hash
 * of the relative path as well, the starting dir's path passed as paramater
 * basepath
 */
#define BUFFERSIZE 65536

void
Jacksum::traverseDirForJacksum(const QString& dirpath,
                            QCryptographicHash *hash, const QString& basepath){

    QDir rootdir(dirpath);
    //qDebug() << "traverseDirForJacksum" << dirpath;
    QFileInfoList filelist = rootdir.entryInfoList(
                    QDir::NoDotAndDotDot|QDir::AllDirs|QDir::Files|QDir::Hidden,
                    QDir::Name|QDir::IgnoreCase|QDir::DirsLast);
    for(int i=0;i < filelist.size(); ++i)
    {
        QFileInfo file = filelist.at(i);
        QString path = file.absoluteFilePath();
        if(file.isDir()){
            traverseDirForJacksum(file.absoluteFilePath(), hash, basepath);
        }
        else{
            QFile f(path);
            char data[BUFFERSIZE];
            if (!f.open(QIODevice::ReadOnly))  {
                //LOG_ERROR("Failed to open file. %s",qPrintable(path));
                //just returning here, which makes the hash check faulty anyway
                return;
            }
            while (!f.atEnd()) {
//                if (isAborted()){
//                    f.close();
//                    return;
//                }
                int length = f.read(data, BUFFERSIZE);
                if (length == -1){
                    //LOG_ERROR("Error during file reading...");
                    return;
                }
                hash->addData(data, length);
                //setProgressBytes(length);
            }
            QString relpath = path.mid(basepath.length() + 1);
            //LOG_DEBUG("relpath: %s", qPrintable(relpath));
            //qDebug() << "relpath" << relpath;
            hash->addData(relpath.toLatin1());
        }
    }
}
