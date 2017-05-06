#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <QObject>
#include <QHash>
#include <QFile>
#include <QXmlStreamReader>
#include <QStringList>

typedef QHash         <QString, QStringList>  String2List_t;
typedef QHashIterator <QString, QStringList>  String2ListIter_t;

class mediaInfo : public QObject
{
    Q_OBJECT
public:
    explicit mediaInfo(QObject *parent = 0);

    bool dumpInfo(QByteArray xml, QString repDir);
    bool loadInfo(QByteArray xml);
    bool buildInfoFile();
    bool buildInfoFile(QByteArray line, QString repDir);
    bool buildDiskInfoFile(QString dir);
    QString getValue(QString name) { return mInfoMap[name]; }

private:
    void conv2sql(QString name,  QXmlStreamAttributes sa);
    QHash<QString, QString> mInfoMap;
    QHash<QString, QString> mMetaMap;
    QStringList mSql;
    QString mt;
    QString mRepDir;

    void writeInfo(QFile &f, const char *name, const char *value_ref);
    bool buildMetaFile();
    void doMediaset();
    void doSwpn();
    void doAtaSection();
    void doAtaChapter();
    void doCustomer();
    void doAcModel();
    void doSwpnAcRef();
    void doAircraft();
    void dumpAttr(QXmlStreamAttributes &sa);
    void buildSqlStart();
    void buildSqlEnd();

    String2List_t mMap1Customer;
    void setCustomer(QString cust,QString model,QString tailsign);
    void dumpHashes();
    void transSwpn2FS(QString & pnfs) {
        pnfs.replace('/', '%');
        pnfs.replace(' ', '@');
    }

signals:
    
public slots:
    
};

#endif // MEDIAINFO_H
