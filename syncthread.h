#ifndef SYNCTHREAD_H
#define SYNCTHREAD_H

#include <QThread>

class SyncThread : public QThread
{
    Q_OBJECT
public:
    explicit SyncThread(QObject *parent = 0);
    
signals:
    
public slots:
    void mMySleep(int msecs);

};

#endif // SYNCTHREAD_H
