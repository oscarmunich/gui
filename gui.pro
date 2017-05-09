folder_01.source = qml/TestQML
folder_01.target = qml
#folder_02.source = qml/colibri
#folder_02.target = qml
folder_03.source = qml/js
folder_03.target = qml
folder_04.source = configs
folder_04.target = .
folder_05.source = scripts
folder_05.target = .
#folder_06.source = qml/colibri_mod
#folder_06.target = qml
folder_07.source = resources
folder_07.target = .
DEPLOYMENTFOLDERS = folder_01 folder_03 folder_04 folder_05 folder_07
TEMPLATE = app

QT      += qml quick
QT      += core gui
QT      += network
QT      += sql
QT      += widgets

CONFIG += c++11

SOURCES += main.cpp \
    Backend.cpp \
    bthread.cpp \
    checkrepository.cpp \
    config.cpp \
    cvaluesourcebase.cpp \
    dbsettings.cpp \
    dbsqlite.cpp \
    fileinfo.cpp \
    loadclient.cpp \
    loadinfo.cpp \
    loadsyncclient.cpp \
    mediainfo.cpp \
    monitorserver.cpp \
    qmyapplication.cpp \
    runclient.cpp \
    setting.cpp \
    signalsock.cpp \
    swpnmatch.cpp \
    syncthread.cpp \
    helpers/confighelper.cpp \
    helpers/keyboardhelper.cpp \
    models/dataloadmodel.cpp \
    network/configuration.cpp \
    network/lanconfiguration.cpp \
    network/wificonfiguration.cpp \
    qmlapplicationviewer/qmlapplicationviewer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
# Default rules for deployment.
#include(deployment.pri)

HEADERS += \
    Backend.h \
    backendbase.h \
    bthread.h \
    build_number.h \
    checkrepository.h \
    config.h \
    cvaluesourcebase.h \
    dbsettings.h \
    dbsqlite.h \
    fileinfo.h \
    jacksum.h \
    loadclient.h \
    loadinfo.h \
    loadsyncclient.h \
    mediainfo.h \
    monitorserver.h \
    qmyapplication.h \
    runclient.h \
    setting.h \
    signalsock.h \
    swpnmatch.h \
    syncthread.h \
    helpers/confighelper.h \
    helpers/keyboardhelper.h \
    models/dataloadmodel.h \
    network/configuration.h \
    network/lanconfiguration.h \
    network/wificonfiguration.h \
    qmlapplicationviewer/qmlapplicationviewer.h
