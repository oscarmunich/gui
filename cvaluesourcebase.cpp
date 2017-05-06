
#include "cvaluesourcebase.h"
#include <QDebug>
#include "swpnmatch.h"
#include <QDebug>
#include <QFile>
#include "Backend.h"

swpnMatch gsw;

QMap<QString, int> CValueSourceBase::mCharMap = {
    { "0", 0 },
    { "1", 1 },
    { "2", 2 },
    { "3", 3 },
    { "4", 4 },
    { "5", 5 },
    { "6", 6 },
    { "7", 7 },
    { "8", 8 },
    { "9", 9 },
    { "A", 10 },
    { "B", 11 },
    { "C", 12 },
    { "D", 13 },
    { "E", 14 },
    { "F", 15 },
    { "G", 16 },
    { "H", 17 },
    { "I", 18 },
    { "J", 19 },
    { "K", 20 },
    { "L", 21 },
    { "M", 22 },
    { "N", 23 },
    { "O", 24 },
    { "P", 25 },
    { "Q", 26 },
    { "R", 27 },
    { "S", 28 },
    { "T", 29 },
    { "U", 30 },
    { "V", 31 },
    { "W", 32 },
    { "X", 33 },
    { "Y", 34 },
    { "Z", 35 },
    { "/", 36 },
    { "-", 37 },
    { " ", 38 },
    { ".", 39 },
    { "_", 40 },
    { "", -1 }
};

CValueSourceBase::CValueSourceBase()
{
    init();
}

void CValueSourceBase::init() {
    gsw.load(gBackend->getDB().getSwpnList());
    gsw.setMap(&mCharMap);
    QString t;
    setSwpn(gsw.match("", t));
    mSwpnIsEnabled = gsw.getChoiceMap();
    setIsEnabled(mSwpnIsEnabled);
}

void CValueSourceBase::keyPressed(const QString &in) {
    if (mIsTailsign)
        keyPressedTail(in);
    else
        keyPressedSwpn(in);
}

void CValueSourceBase::keyPressedTail(const QString &in) {
    qDebug() << "\nCValueSourceBase::keyPressedTail: " << in;
    QString choice;
    QString tail = getTailsign();
    if (mCharMap[in] < 0) {
        qDebug() << "CValueSourceBase::keyPressedTail: backspace";
        if (tail.size() > 0)
            tail = tail.left(tail.size() - 1);
    } else {
        tail.append(in);
    }
    mTailVal = (tail.isEmpty()) ? SWPN_NOTSET : SWPN_MATCH;
    emit isValidChanged();

    setTailsign(tail);
}


void CValueSourceBase::keyPressedSwpn(const QString &in) {
    qDebug() << "\nCValueSourceBase::keyPressedSwpn: " << in;
    QString choice;
    QString swpn = getSwpn();
    if (mCharMap[in] < 0) {
        qDebug() << "CValueSourceBase::keyPressed: backspace";
        swpn = gsw.backspace(swpn, choice);
    } else {
        swpn = gsw.match(swpn + in, choice);
    }
    mSwpnVal = (swpn.isEmpty()) ? SWPN_NOTSET
                                : (gsw.isMatch()) ? SWPN_MATCH : SWPN_VALID;
    emit isValidChanged();
    setSwpn(swpn);
    qDebug() << "CValueSourceBase::swpnval: matches:" << gsw.getMatches();
    qint64 cm = -1;
    cm = gsw.getChoiceMap();
    setIsEnabled(cm);
}

