#include <QDebug>
#include "bthread.h"

BThread::BThread(QObject *parent) :
    QObject(parent)
{
}

// --- DECONSTRUCTOR ---
BThread::~BThread() {
}

void BThread::process() {
    // allocate resources using new here
    qDebug("Hello World!");
    //emit finished();
}

void BThread::receiveString(QString message) {
    qDebug("Hello World! again");
    qDebug() << "receiveString:" << message;
}
