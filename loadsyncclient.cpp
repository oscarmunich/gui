#include "loadsyncclient.h"
#include <QScrollBar>
#include <QStyle>
#include <QToolButton>
#include <QtNetwork/QSslCipher>
#include <QFile>
#include <QDebug>
#include <QIODevice>
#include "Backend.h"

loadSyncClient::loadSyncClient(QObject *parent)
    : QObject(parent)
{
    script     = "/Users/kevin/Development/Qt/build-pdlsync-Desktop_Qt_5_5_1_clang_64bit-Debug/pdlsync";
    process    = NULL;
}

loadSyncClient::~loadSyncClient()
{
    if (process)
        delete process;
}

void loadSyncClient::readFromStdOut()
{
    QString mess = process->readAllStandardOutput();
    //qDebug() << "STDOUT:" << mess;
    procMessage(mess);
}

void loadSyncClient::readFromStdErr()
{
    QString mess = process->readAllStandardError();
    //qDebug() << "STDERR:" << mess;
    procMessage(mess);
}

void loadSyncClient::finishedApp()
{
    qDebug() << "finishedApp";
    gBackend->syncFinished();
}

void loadSyncClient::runCommand(QString command)
{
    /* Make sure that the old process will be deleted later */
    if (process)
        process->deleteLater();

    process = new QProcess();

    /* Connect to readyReadStandardOutput to get the output */
    connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdOut()));
    connect(process, SIGNAL(readyReadStandardError()),  this, SLOT(readFromStdErr()));
    /* Connect to the finished(int) signal to get when the application is done */
    connect(process, SIGNAL(finished(int)), this, SLOT(finishedApp()));
    connect(this,    SIGNAL(sendMessage(QString)), gBackend, SLOT(addLog(QString)));

    /* Process start will start the app in this object */
    QString tcom = script + command;
    qDebug() << "Running:" + tcom;
    process->start(tcom);
}


void loadSyncClient::procMessage(QString &mess)
{
    // Multiple lines with "LOG <type> <data>"\n
    QStringList sl = mess.split("\n");
    //qDebug() << sl;
    for (int i = 0; i < sl.count(); i++) {
        if (!sl[i].startsWith("\"LOG"))
            continue;
        QString t = sl[i].remove(0,5);
        t = t.remove(-1, 1);
        qDebug() << "edit:" << t;
        emit sendMessage(t);
    }
}



