#ifndef SWPNMATCH_H
#define SWPNMATCH_H
#include <QString>
#include <QList>
#include <QMap>

class swpnMatch
{
public:
    swpnMatch();

    void load(QString name);
    void load(QStringList &list) {
        m_qsl = list;
        m_loaded = true;
    }

    void clear();
    void append(QString swpn);

    QString match(QString swpn, QString &choice);
    QString backspace(QString swpn, QString &choice);
    void setMap(QMap<QString, int> *map) {
        mCharMap = map;
    }
    quint64 getChoiceMap() { return mChoiceMap; }
    int getMatches() { return mMatches; }
    void calcChoiceMap();

    bool isMatch() { return mMatch; }
    bool isLoaded() { return m_loaded; }
private:
    QList<QString> m_qsl;
    bool mMatch;
    QString mChoices;
    QMap<QString, int> *mCharMap = NULL;
    bool m_loaded = false;

    int getUnique(QString swpn);
    quint64 mChoiceMap = 0;
    int mMatches = 0;
};

#endif // SWPNMATCH_H
