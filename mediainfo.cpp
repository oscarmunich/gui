#include "mediainfo.h"
#include "fileinfo.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QHash>
#include <QHashIterator>

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

mediaInfo::mediaInfo(QObject *parent) :
    QObject(parent)
{
}

bool mediaInfo::buildDiskInfoFile(QString dir) {
    QDir d;
    qDebug() << "buildDiskInfoFile" << dir;
    d.mkpath(dir);

    QString file = dir + "/." + mInfoMap["filename"].left(7);
    QFile f(file);

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open:<" + file + ">";
        return false;
    }

    char buf[256];
    QHashIterator<QString, QString> qi(mInfoMap);

    while (qi.hasNext()) {
        qi.next();

        SNPRINTF(buf, 256, "media.%s|%s\n", (const char*)qi.key().toUtf8(), (const char*)qi.value().toUtf8());

        QString r = QString::fromLatin1(buf);
        //qDebug() << r.toUtf8();
        f.write((const char*)r.toUtf8());
    }
    f.close();
    return true;
}

bool mediaInfo::buildInfoFile() {
    QDir d;
    QString pnfs = mMetaMap["swpn.swpn"];
    transSwpn2FS(pnfs);
    QString dir = mRepDir + "/" + pnfs;
    //qDebug() << "buildInfoFile" << dir;
    d.mkpath(dir);

    QString file = dir + "/.info";
    QFile f(file);

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open:<" + file + ">";
        return false;
    }
    writeInfo(f, "avg_load_time", "swpn.avg_load_time");
    writeInfo(f, "descriptor",    "swpn.title");
    writeInfo(f, "media_count",   "swpn.media_count");

    f.close();
    return true;
}

// INSERT INTO vw_pdl_swpn VALUES ("DLH1-1307-01", 86, "Nav Database", 3, "M. Fliegner (2013/07/02)");VALUES ("DLH1-1307-01", 86, "Nav Database", 3, "M. Fliegner (2013/07/02)");

bool mediaInfo::buildInfoFile(QByteArray line, QString repDir) {
    if (!line.startsWith("INSERT INTO vw_pdl_swpn "))
        return false;

    //qDebug() << "buildInfoFile" << line;

    mRepDir = repDir;
    int idx;
    int len = line.length();
    for (idx=0; idx < len; idx++) {
        if (line[idx] == '"') {
            break;
        }
    }
    idx++;

    QByteArray t;
    for (; idx < len; idx++) {
        if (line[idx] == '"') {
            break;
        }
        t.append(line[idx]);
    }
    idx+=3;
    mMetaMap["swpn.swpn"] = t;
    //qDebug() << "swpn.swpn" << t;


    t.clear();
    for (; idx < len; idx++) {
        if (line[idx] == ',') {
            break;
        }
        t.append(line[idx]);
    }
    idx+=3;
    mMetaMap["swpn.avg_load_time"] = t;
    //qDebug() << "swpn.avg_load_time" << t;

    t.clear();
    for (; idx < len; idx++) {
        if (line[idx] == '"') {
            break;
        }
        t.append(line[idx]);
    }
    idx+=3;
    mMetaMap["swpn.title"] = t;
    //qDebug() << "swpn.title" << t;

    t.clear();
    for (; idx < len; idx++) {
        if (line[idx] == ',') {
            break;
        }
        t.append(line[idx]);
    }
    mMetaMap["swpn.media_count"] = t;
    //qDebug() << "swpn.media_count" << t;


//    QDir d;
//    QString pnfs = mMetaMap["swpn.swpn"];
//    transSwpn2FS(pnfs);
//    QString dir = mRepDir + "/" + pnfs;
//    qDebug() << "buildInfoFile" << dir;
//    d.mkpath(dir);

//    QString file = dir + "/.info";
//    QFile f(file);

//    if (!f.open(QIODevice::WriteOnly)) {
//        qDebug() << "Can't open:<" + file + ">";
//        return false;
//    }
//    writeInfo(f, "avg_load_time", "swpn.avg_load_time");
//    writeInfo(f, "descriptor",    "swpn.title");
//    writeInfo(f, "media_count",   "swpn.media_count");

//    f.close();
//    return true;
    return buildInfoFile();
}

void mediaInfo::writeInfo(QFile &f, const char *name, const char *value_ref) {
    char buf[256];
    SNPRINTF(buf, 256, "%s %s\n", (const char*)name, (const char*)mMetaMap[value_ref].toUtf8());
    QString r = QString::fromLatin1(buf);
    //qDebug() << r.toUtf8();
    f.write((const char*)r.toUtf8());
}

bool mediaInfo::loadInfo(QByteArray xml) {
    QString name;
    QXmlStreamReader sr(xml);
    //qDebug() << "ESM Meta" << xml;

    //bool inMedia = false;

    while (!sr.atEnd()) {
          sr.readNext();
          switch (sr.tokenType()) {
          case QXmlStreamReader::StartElement:
              name = sr.name().toString();
              //if (name == "media") {
              //    inMedia = true;
              //    continue;
              //}
              continue;
          case QXmlStreamReader::EndElement:
              name = sr.name().toString();
              //if (name == "media") {
              //    inMedia = false;
              //    continue;
              //}
              continue;
          case QXmlStreamReader::Characters:
              //if (!inMedia)
              //    continue;
              mInfoMap[name] = sr.text().toString();
              qDebug() << "SET" << name << mInfoMap[name];
              break;
          default:
              break;
          }
    }
    return false;
}

// TBD Need to do the swpns associated with the mediaset.
void mediaInfo::doMediaset() {
    // Add fields in new version of protocol.
    mMetaMap["mediaset.valid_from"].truncate(10);
    mMetaMap["mediaset.valid_until"].truncate(10);

    QString t = mt.sprintf("INSERT INTO vw_pdl_mediaset VALUES ( \"%s\", \"%s\", %d, \"\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");",
            (const char*)mMetaMap["mediaset.partnumber"].toUtf8(),
            (const char*)mMetaMap["mediaset.title"].toUtf8(),
            (int)mMetaMap["mediaset.media_count"].toInt(),
            (const char*)mMetaMap["mediaset.media_description1"].toUtf8(),
            (const char*)mMetaMap["mediaset.media_description2"].toUtf8(),
            (const char*)mMetaMap["mediaset.loading_standard"].toUtf8(),
            (const char*)mMetaMap["mediaset.valid_from"].toUtf8(),
            (const char*)mMetaMap["mediaset.valid_until"].toUtf8()
                           );
    qDebug() << t;
    mSql.append(t);
    buildInfoFile();
}

void mediaInfo::doSwpn() {
    // Add fields in new version of protocol.
    if (!mMetaMap.contains("swpn.media_description1")) {
        mMetaMap["swpn.media_description1"]="";
    }
    if (!mMetaMap.contains("swpn.media_description2")) {
        mMetaMap["swpn.media_description2"]="";
    }
    if (!mMetaMap.contains("swpn.loading_standard")) {
        mMetaMap["swpn.loading_standard"]="";
    }
    if (!mMetaMap.contains("swpn.valid_from")) {
        mMetaMap["swpn.valid_from"]="2000-01-01";
    }
    if (!mMetaMap.contains("swpn.valid_until")) {
        mMetaMap["swpn.valid_until"]="2099-12-31";
    }
    mMetaMap["swpn.valid_from"].truncate(10);
    mMetaMap["swpn.valid_until"].truncate(10);

    QString t = mt.sprintf("INSERT INTO vw_pdl_swpn VALUES ( \"%s\", %d, \"%s\", %d, \"\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");",
            (const char*)mMetaMap["swpn.swpn"].toUtf8(),
            (int)mMetaMap["swpn.avg_load_time"].toInt(),
            (const char*)mMetaMap["swpn.title"].toUtf8(),
            (int)mMetaMap["swpn.media_count"].toInt(),
            (const char*)mMetaMap["swpn.media_description1"].toUtf8(),
            (const char*)mMetaMap["swpn.media_description2"].toUtf8(),
            (const char*)mMetaMap["swpn.loading_standard"].toUtf8(),
            (const char*)mMetaMap["swpn.valid_from"].toUtf8(),
            (const char*)mMetaMap["swpn.valid_until"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);
    buildInfoFile();
}

void mediaInfo::doAtaSection() {
    QString t = mt.sprintf("INSERT INTO vw_pdl_atasection VALUES ( \"%s\", \"%s\", \"%s\", \"%s\");",
                           (const char*)mMetaMap["acmodel.name"].toUtf8(),
                           (const char*)mMetaMap["ata_section.ata_ref"].toUtf8(),
                           (const char*)mMetaMap["ata_section.number"].toUtf8().right(2),
                           (const char*)mMetaMap["ata_section.name"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);
}

void mediaInfo::doAtaChapter() {
    QString t = mt.sprintf("INSERT INTO vw_pdl_atachapter VALUES ( \"%s\", \"%s\");",
                           (const char*)mMetaMap["ata_chapter.number"].toUtf8(),
                           (const char*)mMetaMap["ata_chapter.descriptor"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);


}
void mediaInfo::doCustomer() {
    QString t = mt.sprintf("INSERT INTO vw_pdl_airline VALUES ( \"%s\", \"%s\");",
                           (const char*)mMetaMap["customer.customer3lc"].toUtf8(),
                           (const char*)mMetaMap["customer.name"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);
}
void mediaInfo::doAcModel() {
}
void mediaInfo::doSwpnAcRef() {
    QString t = mt.sprintf("INSERT INTO vw_pdl_swpn_ac VALUES ( \"%s\", \"%s\", \"%s\", \"%s\", \"%s\");",
                           (const char*)mMetaMap["customer.customer3lc"].toUtf8(),
                           (const char*)mMetaMap["aircraft.name"].toUtf8(),
                           (const char*)mMetaMap["swpn_ac_ref.ata_section_ref"].toUtf8().left(2),
                           (const char*)mMetaMap["swpn_ac_ref.ata_section_ref"].toUtf8().right(2),
                           (const char*)mMetaMap["swpn_ac_ref.name"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);
}
void mediaInfo::doAircraft() {
    //setCustomer(mMetaMap["customer.customer3lc"], mMetaMap["acmodel.name"], mMetaMap["aircraft.name"]);
    QString t = mt.sprintf("INSERT INTO vw_pdl_ac VALUES (\"%s\", \"%s\", \"%s\");",
                           (const char*)mMetaMap["customer.customer3lc"].toUtf8(),
                           (const char*)mMetaMap["acmodel.name"].toUtf8(),
                           (const char*)mMetaMap["aircraft.name"].toUtf8()
                           );
    //qDebug() << t;
    mSql.append(t);
}

void mediaInfo::dumpHashes() {
    String2List_t iter(mMap1Customer);
    QListIterator<QString> it (iter.uniqueKeys());
    while(it.hasNext()) {
        QString k = it.next();
        qDebug() << "Hash" << k << mMap1Customer[k];
        QStringList sl = mMap1Customer[k];
        QStringListIterator it2(sl);
        while(it2.hasNext()) {
            QString k2 = it2.next();
            qDebug() << "    " << k2;
        }

    }
}

void mediaInfo::setCustomer(QString cust,QString model,QString tailsign) {
    QString key = cust + '#' + model;
    if (!mMap1Customer.contains(key)) {
        mMap1Customer[key] = QStringList();
    }
    QStringList *pss = &mMap1Customer[key];
    pss->append(tailsign);
}

void mediaInfo::conv2sql(QString name,  QXmlStreamAttributes sa) {
    QXmlStreamAttribute  attr;
    qDebug() << "name:" << name;
    for (int i=0; i < sa.count(); i++) {
        attr = sa[i];
        QString qualname   = name + '.' + attr.name().toString();
        mMetaMap[qualname] = attr.value().toString();
        qDebug() << "qualname:" << qualname << attr.value().toString();
    }
    if (name == "customer") {
        doCustomer();
        return;
    }
    if (name == "swpn") {
        if (sa.count() >= 4)
            doSwpn();
        return;
    }
    if (name == "mediaset") {
        if (sa.count() >= 4)
            doMediaset();
        return;
    }
    if (name == "aircraft") {
        doAircraft();
        return;
    }
    if (name == "swpn_ac_ref") {
        doSwpnAcRef();
        return;
    }
    if (name == "ata_section") {
        doAtaSection();
        return;
    }
    if (name == "ata_chapter") {
        doAtaChapter();
        return;
    }
}

void mediaInfo::dumpAttr(QXmlStreamAttributes &sa) {
    QXmlStreamAttribute  attr;
    for (int i=0; i < sa.count(); i++) {
        attr = sa[i];
        qDebug() << "Attr:" << attr.name() << attr.value();
    }
}

bool mediaInfo::dumpInfo(QByteArray xml, QString repDir) {
    QString name;
    mRepDir = repDir;
    QXmlStreamReader sr(xml);
    QXmlStreamAttributes sa;
    bool inMediaSet = false;
    QStringList swpns;

    buildSqlStart();

    while (!sr.atEnd()) {
          sr.readNext();
          switch (sr.tokenType()) {
          case QXmlStreamReader::StartElement:
              name = sr.name().toString();
              sa = sr.attributes();
              qDebug() << "StartElement" << name << sr.text().toString();
              if (!inMediaSet and name == "mediaset") {
                  inMediaSet = true;
                  qDebug() << "inMediaSet" << inMediaSet << name;
              }
              conv2sql(name, sr.attributes());
              continue;
          case QXmlStreamReader::EndElement:
              name = sr.name().toString();
              if (inMediaSet and name == "mediaset") {
                  inMediaSet = false;
                  qDebug() << "inMediaSet" << inMediaSet << name;
                  int seq = 0;
                  while (!swpns.isEmpty()) {
                      QString tswpn = swpns.front();
                      qDebug() << "SWPNS" << mMetaMap["mediaset.partnumber"] << seq << tswpn;
                      QString t = mt.sprintf("INSERT INTO vw_pdl_mediaset_swpn VALUES ( \"%s\", %d, \"%s\");",
                              (const char*)mMetaMap["mediaset.partnumber"].toUtf8(),
                              seq++,
                              (const char*)tswpn.toUtf8());
                      swpns.removeFirst();
                      qDebug() << t;
                      mSql.append(t);
                  }
              }
              qDebug() << "EndElement" << name << sr.text().toString();
              continue;
          case QXmlStreamReader::Characters:
              if (inMediaSet and name == "swpn") {
                  qDebug() << "inmediaset swpn" << sr.text().toString();
                  swpns.append(sr.text().toString());
              }
              qDebug() << "Characters" << name << sr.text().toString();
              break;
          default:
              qDebug() << "Token:" << sr.tokenType();
              break;
          }
    }

    buildSqlEnd();

    sr.clear();

    dumpHashes();

    buildMetaFile();

    return false;
}

bool mediaInfo::buildMetaFile() {
    FileInfo mFi;
    QString file = mFi.getDataFileName();
    QFile f(file);

    if (!f.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open:<" + file + ">";
        return false;
    }

    for (int i=0; i < mSql.count(); i++) {
        mSql[i] += "\n";
        f.write((const char*)mSql[i].toUtf8());
    }
    f.close();
    return true;
}

void mediaInfo::buildSqlStart() {
    mSql.clear();

    mSql.append("DROP INDEX IF EXISTS vw_pdl_ac.unq_ac_iata_acm_tailsign;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_ac;");
    mSql.append("CREATE TABLE vw_pdl_ac ( iata_code, acmodel, tailsign );");
    mSql.append("DROP INDEX IF EXISTS vw_pdl_airline.unq_a_iata;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_airline;");
    mSql.append("CREATE TABLE vw_pdl_airline ( iata_code, airline );");
    mSql.append("DROP INDEX IF EXISTS vw_pdl_atachapter.unq_achap_atachapter_no;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_atachapter;");
    mSql.append("CREATE TABLE vw_pdl_atachapter ( atachapter_no, atachapter );");
    mSql.append("DROP INDEX IF EXISTS vw_pdl_atasection.unq_asec_acm_achap_asec;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_atasection;");
    mSql.append("CREATE TABLE vw_pdl_atasection ( acmodel, atachapter_no, atasection_no, atasection );");
    mSql.append("DROP INDEX IF EXISTS vw_pdl_swpn.unq_s_swpn;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_swpn;");
    mSql.append("CREATE TABLE vw_pdl_swpn ( swpn, dl_time, title, media_count, coc_form1_issuer, description1, description2, loading_standard, valid_from, valid_until );");

    mSql.append("DROP INDEX IF EXISTS vw_pdl_mediaset.unq_s_mediaset;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_mediaset;");
    mSql.append("CREATE TABLE vw_pdl_mediaset ( mediaset, title, media_count, coc_form1_issuer, description1, description2, loading_standard, valid_from, valid_until );");

    mSql.append("DROP TABLE IF EXISTS vw_pdl_mediaset_swpn;");
    mSql.append("CREATE TABLE vw_pdl_mediaset_swpn ( mediaset, seq, swpn );");

    mSql.append("DROP INDEX IF EXISTS vw_pdl_swpn_ac.unq_sa_tailsign_asec_swpn;");
    mSql.append("DROP TABLE IF EXISTS vw_pdl_swpn_ac;");
    mSql.append("CREATE TABLE vw_pdl_swpn_ac ( iata_code, tailsign, atachapter_no, atasection_no, swpn );");

    mSql.append("VACUUM;");
    mSql.append("PRAGMA foreign_keys=OFF;");
    mSql.append("BEGIN TRANSACTION;");
}

void mediaInfo::buildSqlEnd() {
    mSql.append("COMMIT;");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_ac_iata_acm_tailsign  ON vw_pdl_ac         ( iata_code, acmodel, tailsign );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_a_iata                ON vw_pdl_airline    ( iata_code );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_achap_atachapter_no   ON vw_pdl_atachapter ( atachapter_no );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_asec_acm_achap_asec   ON vw_pdl_atasection ( atachapter_no, atasection_no, acmodel );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_s_swpn                ON vw_pdl_swpn       ( swpn );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_s_partnumber          ON vw_pdl_mediaset   ( partnumber );");
    mSql.append("CREATE UNIQUE INDEX IF NOT EXISTS unq_sa_tailsign_asec_swpn ON vw_pdl_swpn_ac    ( tailsign, atasection_no, swpn );");
}
