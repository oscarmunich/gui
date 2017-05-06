#ifndef CHECKREPOSITORY_H
#define CHECKREPOSITORY_H

#include <QObject>
#include <QStringList>
#include "syncthread.h"

class  CheckRepository;
extern CheckRepository *gCheckRep;

class CheckRepository : public QObject
{
    Q_OBJECT
public:
    explicit CheckRepository(QObject *parent = 0);
    bool checkAll(bool force = false);
    bool checkPartNumber(QString partnumber, bool force = false);
    bool checkPartNumber4Load(QString partnumber);
    QString pn2fs(QString pn);
    QString fs2pn(QString pn);
    static void createCheckThread();

signals:
    void PNOk(QString, bool);

public slots:
    void checkPN(QString pn);

private:
    bool doPartNumberCheck(QString partnumber);
    bool fileExists(QString file);
    bool removeFile(QString file);
    bool createFile(QString file);
    bool dirExists(QString & file);
    QString mRepPath;
    QString mPartNumber;
    QString mPartNumberDir;
    QString mMediaCount;
    QString mChecksum;
    static QString mVerified;
    static QString mInfo;
    QStringList mDotDisk;
    QStringList mZipDisk;
    qint64 mFileSize;
    bool getAttributes(const QString & file);
    bool checkSize(const QString & file);
    bool checkChecksum(const QString & file);
    QString getFilePath(const QString & file);
    SyncThread *mCheckThread;
};

#endif // CHECKREPOSITORY_H
