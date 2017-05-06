#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QList>
#include <QDebug>

typedef enum {
    CONF_PROD,
    CONF_EESM_TEST_INTRA,
    CONF_EESM_PROD_INTRA,
    CONF_EESM_TEST,
    CONF_EESM_PROD,
    CONF_FLSD_TEST,
    CONF_FLSD_PROD,
    CONF_TEST_LOCAL,
    CONF_TEST_1UND1,
    CONF_FLSD_TEST_DEV,
    CONF_FLSD_PROD_DEV,
    CONF_STUNNEL,
    CONF_MAX,
} conf_type_t;

class config
{
public:
    config();
    config(conf_type_t type);
    config & getConfig(conf_type_t type);
private:
    static QList<config> mConfigs;
public:
    void setURL(QString & server) {
        mConfigs[mCurrConf].mURL = server;
        qDebug() << "setURL" << server;
    }
    QString getURL()  { return getCurr().mURL; }
    QString getName() { return getCurr().mName; }
    QString getPost() { return getCurr().mPost; }
    QString getCert() { return getCurr().mCert; }
    int  getPort()    { return getCurr().mPort; }
    bool getSecure()  { return getCurr().mSecure; }
    bool getSimload() { return getCurr().mSimload; }
    bool isFLSDesk()  { return getCurr().mFLSDesk; }
    bool isClInfo()   { return getCurr().mClInfo; }
    void setConfig(conf_type_t conf) { mCurrConf = conf; }
private:
    const config &getCurr() { return mConfigs[mCurrConf]; }
    QString mName;
    QString mURL;
    QString mPost;
    QString mCert;
    int mPort;
    bool mSecure;
    bool mSimload;
    bool mFLSDesk;
    bool mClInfo;
private:
    conf_type_t mCurrConf;

};

extern config *gConfig;
#endif // CONFIG_H
