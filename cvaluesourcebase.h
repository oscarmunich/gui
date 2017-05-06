/*
 * This file is generated from the following input
 *
 * using python version 3.5.1 |Anaconda 4.0.0 (x86_64)| (default, Dec  7 2015, 11:24:55) 
[GCC 4.2.1 (Apple Inc. build 5577)]
 *  int|isEnabled|-1|Buttons enabled
 *  QStringListModel|swpnListModel|-1|Buttons enabled
 *  QStringList|swpnList|-1|Buttons enabled
 *  QString|swpn|Hello|Actual swpn
 *
*/

#ifndef CVALUESOURCEBASE_H
#define CVALUESOURCEBASE_H

#include <QObject>
#include <QStringList>
#include <QStringListModel>
#include <QDebug>

typedef enum {
        SWPN_NOTSET,
        SWPN_NOK,
        SWPN_VALID,
        SWPN_MATCH,
        TAIL_SET = SWPN_MATCH
} swpnval_t;

class CValueSourceBase : public QObject
{
    Q_OBJECT
    friend class CValueSource;

    Q_PROPERTY(int              isEnabled     READ getIsEnabled                            NOTIFY isEnabledChanged    )
    Q_PROPERTY(int              isEnabled2    READ getIsEnabled2                           NOTIFY isEnabled2Changed   )
    Q_PROPERTY(QString          swpn          READ getSwpn                                 NOTIFY swpnChanged         )
    Q_PROPERTY(QString          tailsign      READ getTailsign                             NOTIFY tailsignChanged     )
    Q_PROPERTY(bool             isTailsign    READ getIsTailsign WRITE setIsTailsign       NOTIFY isTailsignChanged   )
    Q_PROPERTY(bool             isValid       READ getIsValid                              NOTIFY isValidChanged      )

public:
    CValueSourceBase();

private:
    qint64           mSwpnIsEnabled = -1;             // SWPN Buttons enabled
    qint64           mTailIsEnabled = -1;             // TAIL Buttons enabled
    qint64           mIsEnabled     = -1;             // Buttons enabled
    QString          mSwpn ;                          // Actual swpn
    QString          mTailsign ;                      // Tailsign

public:
    qint64              getIsEnabled()      {
       //qDebug() << "getISEnabled" << hex << mIsEnabled << dec;
       return mIsEnabled;
    }
    qint64              getIsEnabled2()      {
       //qDebug() << "getISEnabled2" << hex << mIsEnabled << dec;
       return mIsEnabled >> 32;
    }
    QString          getSwpn()           { return mSwpn; }
    QString          getTailsign()       { return mTailsign; }

    bool             getIsTailsign()      {
       qDebug() << "getIsTailsign" << mIsTailsign;
       return mIsTailsign;
    }
    bool             getIsValid() {
        return (mSwpnVal == SWPN_MATCH and mTailVal == SWPN_MATCH);
    }

public:
    void setIsEnabled(qint64 isEnabled) {
        if (mIsEnabled != isEnabled) {
            mIsEnabled  = isEnabled;
            //qDebug() << "setIsEnabled 1 " << hex << mIsEnabled << dec;
            emit isEnabledChanged();
            emit isEnabled2Changed();
        }
    }
    void setSwpn(QString swpn) {
        if (mSwpn != swpn) {
            mSwpn  = swpn;
            emit swpnChanged();
        }
    }
    void setTailsign(QString tailsign) {
        qDebug() << "setTailsign " << tailsign;
        if (mTailsign != tailsign) {
            mTailsign  = tailsign;
            emit tailsignChanged();
        }
        setIsEnabled((mIsTailsign) ? ((getTailsign().length() == 0) ? 0x003ffffffc00 : 0x003fffffffff) : mSwpnIsEnabled);
    }

    void setIsTailsign(bool istailsign) {
        qDebug() << "setIsTailsign" << istailsign << " " << mIsTailsign;
        if (mIsTailsign != istailsign) {
            mIsTailsign  = istailsign;
            emit isTailsignChanged();
        }
        setIsEnabled((mIsTailsign) ? ((getTailsign().length() == 0) ? 0x003ffffffc00 : 0x003fffffffff) : mSwpnIsEnabled);
    }

    Q_INVOKABLE QString getSwpnStateColor() {
        QString color = mColors[(int)getSwpnState()];
        qDebug() << "getSwpnStateColor" << color;
        return color;
    }

    Q_INVOKABLE QString getTailStateColor() {
        QString color = mColors[(int)mTailVal];
        qDebug() << "getTailStateColor" << color;
        return color;
    }

    swpnval_t getSwpnState() {
        qDebug() << "getSwpnState" << (int) mSwpnVal;
        return mSwpnVal;
    }

signals:
    void isEnabledChanged();
    void isEnabled2Changed();
    void swpnChanged();
    void tailsignChanged();
    void isTailsignChanged();
    void isValidChanged();

public:
    void init();

    Q_INVOKABLE void keyPressed(const QString &in);

private:
    void keyPressedTail(const QString &in);
    void keyPressedSwpn(const QString &in);

    static QMap<QString, int> mCharMap;
    QStringList mColors = {  "white", "red", "yellow", "lightgreen", "blue" };
    swpnval_t mTailVal = SWPN_NOTSET;
    swpnval_t mSwpnVal = SWPN_NOTSET;
    bool mIsTailsign = false;
};
#endif // CVALUESOURCEBASE_H

