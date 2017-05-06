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

#include "loadclient.h"
#include "Backend.h"
#include "config.h"

#include <QScrollBar>
#include <QStyle>
#include <QToolButton>
#include <QtNetwork/QSslCipher>
#include <QFile>
#include <QDebug>
#include <QIODevice>
#include "checkrepository.h"

#ifdef Q_WS_WIN32
#define SNPRINTF _snprintf
#include "Windows.h"
#else
#define SNPRINTF ::snprintf
#endif

 LoadClient::LoadClient(QObject *parent)
     : QObject(parent), reply(0)
 {
     script     = mFi.getLoaderName();
     process    = NULL;
     //mMonServer = NULL;
  }

 LoadClient::~LoadClient()
 {
     if (process)
         delete process;
 }


 void LoadClient::updateEnabledState()
 {
 }

 void LoadClient::send2Loader(QString message)
 {
     if (gBackend->mMonServer) {
        gBackend->mMonServer->send2Clients(message);
     }
 }

 void LoadClient::saveReply()
 {
     QString tempfile = mFi.getTempName("reply");
     QFile f(tempfile);
     if (!f.open(QIODevice::WriteOnly)) {
         gBackend->addLog("Can't open:<" + tempfile + ">");
        return;
     }
     f.write(reply);
     f.close();
     reply.clear();
 }

 void LoadClient::synchroniseWithServer(Backend * /* bp */) {
     state = 1;
     //mbp = bp;
     //creatMonitorSocket();
     //gBackend->addLog("synchroniseWithServer");
     procMessage();
 }

 void LoadClient::readFromStdOut()
 {
     QString mess = process->readAllStandardOutput();
     //reply.append(mess);
     gBackend->addLog(mess);
 }

 void LoadClient::finishedApp()
 {
   qDebug() << "reply" << reply;
   procMessage();
 }

 void LoadClient::runCommand(QString command)
 {
   /* Make sure that the old process will be deleted later */
   if (process)
       process->deleteLater();

   process = new QProcess();

   /* Connect to readyReadStandardOutput to get the output */
   connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(readFromStdOut()));
   /* Connect to the finished(int) signal to get when the application is done */
   connect(process, SIGNAL(finished(int)), this, SLOT(finishedApp()));

   /* Process start will start the app in this object */
   QString tcom = script + command;
   qDebug() << "Running:" + tcom;
   process->start(tcom);
 }


  void LoadClient::procMessage()
 {
     QString mess;
     QString t;
     bool ok;
     CheckRepository checkRep;

     qDebug() << "state" << state;
     switch (state) {
     case 1:    // Start data load
         ok = checkRep.checkPartNumber4Load(gBackend->getSoftware());
         mess = t.sprintf("%s Verification L: %s", (const char*) gBackend->getSoftware().toUtf8(), (ok) ? "OK" : "FAILED");
         gBackend->addLog(mess);
         if (!ok) {
             gBackend->addLog("Please abort data load and re-synchronise the PDL");
             break;
         }

         if (gConfig->getSimload() || gBackend->getSoftware().startsWith("SIMULATE")) {
             t = "-S -a \"" + gBackend->getTailsign() + "\" -k \"" + gBackend->getSoftware() + "\""; // Test using simulation
         } else {
             t = "-x -a \"" + gBackend->getTailsign() + "\" -k \"" + gBackend->getSoftware() + "\"";
         }
         gBackend->addLog("Loading " + t);
         runCommand(t);
         state = 9;
         break;

     case 9:
         gBackend->setRunning(false);
         break;
     }

 }



