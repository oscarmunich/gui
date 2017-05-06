#ifndef JACKSUM_H
#define JACKSUM_H

#include <QObject>
#include <QCryptographicHash>

class Jacksum : public QObject
{
    Q_OBJECT
public:
    explicit Jacksum(QObject *parent = 0);

private:
    void traverseDirForJacksum(const QString& dirpath,
                               QCryptographicHash *hash, const QString& basepath);
public:
    bool checkAllSWPNJackums();
    bool checkSWPNJackums(const QString &mSwpn);
    QString getDiskJacksumInfo(const QString &swpn);
    QString calculateDirJacksum(const QString& dirpath);

signals:

public slots:
};

#endif // JACKSUM_H
