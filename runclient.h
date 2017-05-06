/****************************************************************************
 **
 ** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #ifndef RUNCLIENT_H
 #define RUNCLIENT_H

#include <QWidget>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QProcess>
#include "fileinfo.h"

 class QSslSocket;
 class Backend;

 typedef enum {
     RUN_IDLE,
     RUN_STARTUP,
     RUN_RUNNING,
 } run_state_t;


 class RunClient : public QObject
 {
     Q_OBJECT
 public:
     RunClient(QObject *parent = 0);
     ~RunClient();

    void doRun(QString command);
    void send2Loader(QString message);
    void stopCommand();
    int  getReturnCode()        { return mRetCode; }
    QString getCommandOutput()  { return mReply; }
    run_state_t getState()      { return mState; }
    int  getExitStatus()        { return mExitStatus; }

 signals:
    void setStatus(QString text);
    void addLog(QString text);
    void commandResponse(int, QString);
    void commandOutput(QString);

 private slots:
     void readFromStdOut();
     void finishedApp();

 private:
     FileInfo mFi;
     QProcess *mProcess;
     QByteArray mReply;
     QString mCommand;
     int mRetCode;
     int mExitStatus;
     QString mMessage;

     run_state_t mState;

     void runCommand(QString command);
     void procMessage();
     void sendMessage(QString mess);
     void saveReply();
 };

 #endif
