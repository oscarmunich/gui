#include "syncthread.h"

SyncThread::SyncThread(QObject *parent) :
    QThread(parent)
{
    setPriority(QThread::LowPriority);
}

void SyncThread::mMySleep(int msecs) {
    QThread::msleep(msecs);
}
