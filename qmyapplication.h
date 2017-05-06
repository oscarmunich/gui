#ifndef QMYAPPLICATION_H
#define QMYAPPLICATION_H

#include <QApplication>
#include <qcoreapplication.h>
#include <qcoreevent.h>
#include "fileinfo.h"

class QMyApplication : public QApplication
{
public:
    QMyApplication(int &argc, char **argv);
    bool eventFilter(QObject *obj, QEvent *event);
#ifdef HANDLENOTIFY
    bool notify(QObject *obj, QEvent *event);
#endif
private:
    void setEventList();
    FileInfo mFi;

};

#endif // QMYAPPLICATION_H
