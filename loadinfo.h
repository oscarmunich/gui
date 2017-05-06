#ifndef LOADINFO_H
#define LOADINFO_H

#include <QObject>
#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QList>

typedef enum {
    LDI_STARTTIME,
    LDI_END_TIME,
    LDI_LOAD_TYPE,
    LDI_LOAD_STATUS,
    LDI_LOADER_VER,
    LDI_USER_COMMENT,
    LDI_PART_NO,
    LDI_AC_REG,
    LDI_JACKSUM
} ldi_types_t;

class Backend;

class LoadInfo : public QObject
{
    Q_OBJECT
public:
    explicit LoadInfo(QObject *parent = 0);
    
signals:
    
public slots:
public:
    int getCount() { return mDirs.count(); }
    int getZipCount() { return mZips.count(); }
    QStringList getLoadInfo() { return mLoadinfo; }
    QStringList getFileInfo() { return mFileinfo; }
    void setUploaded();
    bool getNextLoad();
    void appendFiles2Message(QByteArray &mess, QString &boundary);
    QString getCurrDir();
    int getCurrCount() { return (mCurr == -1) ? 0 : mCurr; }
    void reset();

private:
    void getFiles2Upload(QString dirname);
    bool getLoadInfo(QString filename);
    bool removeDir(const QString & dirName);

    int mCurr;

    QDir mLoaddir;
    QStringList mDirs;
    QStringList mZips;
    QStringList mLoadinfo;
    QStringList mFileinfo;
};

#endif // LOADINFO_H
