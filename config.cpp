#include "config.h"
#include "fileinfo.h"

class config;
config *gConfig;
config::config()
{
    int e;
    for (e=(int)CONF_PROD; e < (int)CONF_MAX; e++) {
        mConfigs.append(config((conf_type_t)e));
    }
    mCurrConf = CONF_FLSD_TEST;
}

QList<config> config::mConfigs;

config::config(conf_type_t type)
{
    FileInfo mFi;
    switch(type) {
    case CONF_PROD:
        // Temporary fix for .connection being reset to '0', so simply configure wanted connection on '0' as well
        mURL     = "fls-desk.mbs-electronics.com";
        mName    = "FLS-Desk PROD Server (Def)";
        mPost    = "/cgi-bin/flsService.pl";
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = true;
        mCert    = "fls-desk.mbs-electronics.com";
        mClInfo  = false;
        break;
    case CONF_EESM_TEST_INTRA:
        mURL     = "lww.manage-m.lht.ham.dlh.de";
        mName    = "eESM Production Server (intranet)";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = false;
        mCert    = "manage-m.com";
        mClInfo  = true;
        break;
    case CONF_EESM_PROD_INTRA:
        mURL     = "lww.manage-m.lht.ham.dlh.de";
        mName    = "eESM Production Server (intranet)";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = false;
        mCert    = "manage-m.com";
        mClInfo  = true;
        break;
    case CONF_EESM_TEST:
        mURL     = "kons.manage-m.com";
        mName    = "eESM Test Server";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = false;
        mCert    = "manage-m.com";
        mClInfo  = true;
        break;
    case CONF_EESM_PROD:
        mURL     = "www.manage-m.com";
        mName    = "eESM Production Server";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = false;
        mCert    = "manage-m.com";
        mClInfo  = true;
        break;
    case CONF_FLSD_TEST:
        mURL     = "fls-desk.mbs-electronics.com";
        mURL     = "192.168.2.40";
        mName    = "FLS-Desk Test server";
        mPost    = "/cgi-bin/flsService.pl";
        mPort    = 29000;
        mSecure  = false;
        mSimload = false;
        mFLSDesk = true;
        mClInfo  = false;
        break;
    case CONF_FLSD_PROD:
        mURL     = "fls-desk.mbs-electronics.com";
        mName    = "FLS-Desk Prod server (secure)";
        mPost    = "/cgi-bin/flsService.pl";
        mPort    = 29000;
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = true;
        mClInfo  = false;
        break;
    case CONF_STUNNEL:
        mURL     = "192.168.20.10";
        mName    = "eESM Production Server";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 16000;
        mSecure  = false;
        mSimload = false;
        mFLSDesk = false;
        mClInfo  = true;
        break;
    case CONF_FLSD_TEST_DEV: // 9
        mURL     = "fls-desk-dev.mbs-electronics.com";
        mName    = "FLS-Desk Dev Test server";
        mURL     = "fls-desk.mbs-electronics.com";
        mName    = "FLS-Desk Prod server (29001)";
        mPost    = "/cgi-bin/flsService.pl";
        mPort    = 29001;
        mSecure  = false;
        mSimload = false;
        mFLSDesk = true;
        mClInfo  = false;
        break;
    case CONF_FLSD_PROD_DEV: // 10
        mURL     = "fls-desk-dev.mbs-electronics.com";
        mName    = "FLS-Desk Dev server (secure)";
        mPost    = "/cgi-bin/flsService.pl";
        mPort    = 29000;
        mPort    = 443;
        mSecure  = true;
        mSimload = false;
        mFLSDesk = true;
        mClInfo  = false;
        break;
    case CONF_TEST_LOCAL:
        mURL     = "localhost";
        mName    = "local test server";
        mPost    = "/fls-service/services/PdlClientWebService";
        mPort    = 29000;
        mSecure = false;
        mSimload = true;
        mFLSDesk = true;
        mClInfo  = false;
        break;
    case CONF_MAX: break;
    default:       break;
    }
}


