#include "swpnmatch.h"
#include <QFile>
#include <QDebug>
#include <QRegularExpression>

swpnMatch::swpnMatch()
{

}

void swpnMatch::clear() {
    m_qsl.clear();
}

void swpnMatch::append(QString swpn) {
    swpn = swpn.trimmed();
    if (swpn.length() > 0)
        m_qsl.append(swpn);
}

void swpnMatch::load(QString name) {
    char buf[256];

    clear();

    QFile f(name);
    if (!f.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file.";
        return;
    }

    int len;
    while ((len = f.readLine(buf, 256)) != -1) {
        QString line = buf;
        append(line);
    }

    f.close();


}

QString swpnMatch::backspace(QString swpn, QString &choice) {
    mMatch = false;

    qDebug() << "In     <" << swpn << ">";
    swpn.chop(1);
    while (getUnique(swpn) < 2 && swpn.length() > 0 and !mMatch) {
        swpn.chop(1);
    }
    qDebug() << "lp     <" << swpn << ">";

    choice = mChoices;
    qDebug() << "Choice:<" << choice << ">";

    calcChoiceMap();

    return swpn;
}

QString swpnMatch::match(QString swpn, QString &choice) {
    mMatch = false;

    qDebug() << "In     <" << swpn << ">";
    while (getUnique(swpn)) {
        if (mChoices.length() > 1 or mMatch)
            break;
        swpn += mChoices.mid(0,1);
        //qDebug() << "lp     <" << swpn << ">";
    }

    choice = mChoices;
    qDebug() << "Choice:<" << swpn << choice << ">";

    calcChoiceMap();

    return swpn;
}

void swpnMatch::calcChoiceMap() {
    mChoiceMap = 0;
    for (int i = 0; i < mChoices.size(); i++) {
        QString let = mChoices.mid(i,1);
        int val = (*mCharMap)[let];
        mChoiceMap |= ((quint64)1 << val);
        //qDebug() << let << val << hex << mChoiceMap << dec;
    }
    //qDebug() << hex << mChoiceMap << dec;
}

int swpnMatch::getUnique(QString swpn) {
    int length = swpn.length();
    QMap<QString, int> map;
    mChoices.clear();

    QRegularExpression regex;
    regex.setPattern("^" + swpn);

    QList<QString> qslf = m_qsl.filter(regex);
    mMatches = qslf.size();
    QList<QString>::const_iterator i;

    for (i = qslf.constBegin(); i != qslf.constEnd(); ++i) {
        QString t = *i;
        // because of the filter entries must be equal or longer
        if (length == t.length()) {
            mMatch = true;
            continue;
        }
        // pn longer - save next character uniquely in map
        QString c = t.mid(length, 1);
        if (!map.contains(c)) {
            map.insert(c, 1);
            mChoices += c;
            continue;
        }
    }

    return map.count();
}
