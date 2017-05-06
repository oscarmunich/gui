#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QStringList>
#include <QHash>
#include <QHashIterator>

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = 0);
    void test();
    void setValue(const QString & key, const QString & value);
    void dump2File();
    void saveSettings(const QString & input);

signals:
    
public slots:
private:
    QStringList mSql;
    QHash<QString, QString> mInfoMap;

    bool buildSettingsFile();
    void buildSqlStart();
    void buildSqlEnd();

};

#endif // SETTING_H
