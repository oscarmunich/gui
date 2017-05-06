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

#include "runclient.h"
#include "Backend.h"
#include "config.h"

#include <QScrollBar>
#include <QStyle>
#include <QToolButton>
#include <QtNetwork/QSslCipher>
#include <QFile>
#include <QDebug>
#include <QIODevice>

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

RunClient::RunClient(QObject *parent)
    : QObject(parent), mReply(0)
{
    mProcess   = NULL;
    mState     = RUN_IDLE;
}

RunClient::~RunClient()
{
    if (mProcess)
        delete mProcess;
}

void RunClient::send2Loader(QString message)
{
    if (gBackend->mMonServer) {
        gBackend->mMonServer->send2Clients(message);
    }
}

void RunClient::saveReply()
{
    QString tempfile = mFi.getTempName("reply");
    QFile f(tempfile);
    if (!f.open(QIODevice::WriteOnly)) {
        gBackend->addLog("Can't open:<" + tempfile + ">");
        return;
    }
    f.write(mReply);
    f.close();
    mReply.clear();
}

void RunClient::doRun(QString command) {
    mState = RUN_STARTUP;
    mReply.clear();
    mMessage.clear();
    mCommand = command;
    procMessage();
}

void RunClient::readFromStdOut()
{
    QString mess = mProcess->readAllStandardOutput();
    mMessage += mess;
    int index;
    while ((index = mMessage.indexOf("\n")) != -1) {
        QString tmess = mMessage.left(index);
        if (tmess.startsWith("Status:")) {
            tmess.remove(0, 7);
            gBackend->setStatus(tmess);
        } else {
            gBackend->addLog(tmess);
        }
        mMessage.remove(0, index+1);
    }
    mReply.append(mess);
    emit commandOutput(mess);
}

void RunClient::finishedApp()
{
    mRetCode    = mProcess->exitCode();
    mExitStatus = mProcess->exitStatus();
    //qDebug() << "reply" << mReply;
    qDebug() << "exit " << mRetCode;
    qDebug() << "exitStatus " <<  mProcess->exitStatus();
    procMessage();
}

void RunClient::stopCommand()
{
    if (mState != RUN_IDLE)
        mProcess->terminate();
}

void RunClient::runCommand(QString command)
{
    /* Make sure that the old process will be deleted later */
    if (mProcess)
        mProcess->deleteLater();

    mProcess = new QProcess();

    connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdOut()));
    connect(mProcess, SIGNAL(finished(int)),             this, SLOT(finishedApp()));

    /* Process start will start the app in this object */
    qDebug() << "Running:" + command;
    mProcess->start(command);
}


void RunClient::procMessage()
{
    qDebug() << "mState" << mState;
    switch (mState) {
    case RUN_STARTUP:    // Star data load
        gBackend->addLog("running " + mCommand);
        runCommand(mCommand);
        mState = RUN_RUNNING;
        break;
    case RUN_RUNNING:
        mState = RUN_IDLE;
        emit commandResponse(mRetCode, mReply);
        break;
    case RUN_IDLE:
        break;
    }

}



