//#define QT_NO_OBJECTS 1
#include <QApplication>
#include "qmlapplicationviewer/qmlapplicationviewer.h"
#include <QQmlContext>
//#include <QtDeclarative>
#include "Backend.h"
#include <QDebug>
#include <QCursor>
#include "config.h"
#include "fileinfo.h"
#include "dbsqlite.h"
#include "signalsock.h"
#include "./network/lanconfiguration.h"
#include "./network/wificonfiguration.h"
#include "./models/dataloadmodel.h"
#include "./helpers/keyboardhelper.h"
#include "./helpers/confighelper.h"
#include "setting.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "qmyapplication.h"
#include <getopt.h>
#include <mediainfo.h>
#include "jacksum.h"
#include "cvaluesourcebase.h"

#define QApplication QMyApplication

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

Backend *gBackend;
QmlApplicationViewer *gViewer;
static struct sigaction sigact;
QmlApplicationViewer *wviewer;

Q_DECL_EXPORT static void signal_handler(int sig) {
    switch(sig) {
    case SIGINT:
        qDebug() << "Caught signal:SIGINT";
        break;
    case SIGBUS:
        qDebug() << "Caught signal:SIGBUS";
        exit(10);
    case SIGSEGV:
        qDebug() << "Caught signal:SIGSEGV";
        exit(11);
    }
}

Q_DECL_EXPORT void init_signals(void){
    sigact.sa_handler = signal_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigaction(SIGINT,  &sigact, (struct sigaction *)NULL);
    sigaction(SIGBUS,  &sigact, (struct sigaction *)NULL);
    sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL);
}

Q_DECL_EXPORT int main(int argc, char **argv)
{
    qmlRegisterType<DataLoadModel>("Models", 1, 0, "DataLoadModel");
    qmlRegisterType<LanConfiguration>("Network", 1, 0, "LanConfiguration");
    qmlRegisterType<WiFiConfiguration>("Network", 1, 0, "WiFiConfiguration");
    //qmlRegisterUncreatableType<AccessPoint>("Network", 1, 0, "AccessPoint", "Could not instantiate AccessPoint");

    const char *com = *argv;
    qDebug() << "main" << com;
    int opt= 0;
    int long_index =0;

    int syncTime = 0;
    int repeat   = 0;
    QString accessKey;

    static struct option long_options[] = {
        {"accessKey", required_argument,  0, 'a'},
        {"synctime",  required_argument,  0, 't'},
        {"repeat",    required_argument,  0, 'r'},
        {"path",      required_argument,  0, 'p'},
        {"portdelta", required_argument,  0, 'd'},
        {0, 0, 0,0 }
    };
    while ((opt = getopt_long(argc, argv,"a:t:r:p:d:", long_options, &long_index )) != -1) {
        switch (opt) {
        case 'a': accessKey = optarg;
            break;
        case 't': syncTime = atoi(optarg);
            break;
        case 'r': repeat   = atoi(optarg);
            break;
        case 'p': FileInfo::mTestPath = optarg;
            break;
        case 'd': ConfigHelper::m_portdelta = atoi(optarg);
            break;
        }
    }

    FileInfo fi;
    fi.inLoad(false);
    fi.inSync(false);

    //init_signals();
#define Q_WS_DARWIN
#if !defined(Q_WS_WIN32) && !defined(Q_WS_X11) && !defined(Q_WS_MAC) && !defined(Q_WS_DARWIN)
    int xargc = 4;
    const char *xargv[] = {0, "-qws", "-display", "Transformed:rot90", 0 };
    xargv[0] = argv[0];
    qDebug() << "xargv" << xargv[0] << xargv[1] << xargv[2] << xargv[3] << getpid();
    QScopedPointer<QApplication> app(new QApplication(*(&xargc), (char **)xargv));
#else
    qDebug() << "argv" << argv[0] << argv[1] << argv[2] << argv[3] << getpid();
    QScopedPointer<QApplication> app(new QApplication(argc, argv));
#endif

    QCoreApplication::instance()->installEventFilter(app.data());

    qDebug() << "accessKey" << accessKey;
    qDebug() << "syncTime" << syncTime;

    gBackend = new Backend();
    gBackend->Init();
    gBackend->initialiseDatabases();

    if (!accessKey.isEmpty())
        gBackend->setAccessKey(accessKey);
    if (syncTime)
        gBackend->setSyncTimer(syncTime);

    gConfig  = new config();

    conf_type_t actconf;
    int connect_type = fi.getConnection();
    bool usesettings = false;
    switch(connect_type) {
    default:
        actconf = CONF_EESM_PROD_INTRA;
        //usesettings = true;
        break;

    case CONF_PROD:
    case CONF_EESM_TEST_INTRA:
    case CONF_EESM_PROD_INTRA:
    case CONF_EESM_TEST:
    case CONF_EESM_PROD:
    case CONF_FLSD_TEST:
    case CONF_FLSD_PROD:
    case CONF_TEST_LOCAL:
    case CONF_TEST_1UND1:
    case CONF_FLSD_TEST_DEV:
    case CONF_FLSD_PROD_DEV:
    case CONF_STUNNEL:
        actconf = (conf_type_t)connect_type;
        break;
    }

    qDebug() << "Connection" << actconf;

    gBackend->setServerId(actconf);
    gConfig->setConfig(actconf);
    if (usesettings) {
        QString server;
        if (gBackend->getSettingsDB().get_sync_server(server) and !server.isEmpty()) {
            gConfig->setURL(server);
            qDebug() << "server" << server;
        }
    }

    QmlApplicationViewer viewer;
    gViewer = &viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);

    #if !defined(Q_WS_WIN32) && !defined(Q_WS_X11) && !defined(Q_WS_MAC)  && !defined(Q_WS_DARWIN)
      viewer.setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
      viewer.setWindowState(Qt::WindowFullScreen);

      QPixmap nullCursor(16, 16);
      nullCursor.fill(Qt::transparent);
      app->setOverrideCursor( QCursor(nullCursor) );
    #endif


      KeyboardHelper keyboardHelper(&viewer);
    CValueSourceBase cv;

    /* Register backend functions */
    QQmlContext *ctxt = viewer.rootContext();
    ctxt->setContextProperty( "cvaluebase",  &cv);
    ctxt->setContextProperty( "backend",     gBackend);
    ctxt->setContextProperty( "lm_airline",  gBackend->airlineModel());
    ctxt->setContextProperty( "lm_actype",   gBackend->actypeModel());
    ctxt->setContextProperty( "lm_tailsign", gBackend->tailsignModel());
    ctxt->setContextProperty( "lm_software", gBackend->softwareModel());
    //ctxt->setContextProperty( "keyboardHelper", &keyboardHelper);
    ctxt->setContextProperty( "configHelper", ConfigHelper::instance());

    viewer.setMainQmlFile(QLatin1String((fi.isDemoMode()) ? "qml/TestQML/maindemo.qml" :  "qml/TestQML/A0_main.qml"));
    viewer.showExpanded();
    //splash.finish(&viewer);

    signalSock sigsoc;

    int result = app->exec();

    return result;
}
