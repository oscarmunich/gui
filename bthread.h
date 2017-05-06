#ifndef BTHREAD_H
#define BTHREAD_H

#include <QObject>

class BThread : public QObject {
    Q_OBJECT

public:
    explicit BThread(QObject *parent = 0);
    ~BThread();

public slots:
    void process();
    void receiveString(QString message);

signals:
    void finished();
    void error(QString err);

private:
    // add your variables here
};

#endif // BTHREAD_H
