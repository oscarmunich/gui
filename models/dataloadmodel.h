#ifndef DATALOADMODEL_H
#define DATALOADMODEL_H

#include <QSqlQueryModel>

class DataLoadModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit DataLoadModel(QObject *parent = 0);

    Q_INVOKABLE void resetModel();
    Q_INVOKABLE void loadAirlines();
    Q_INVOKABLE void loadActmodels(const QString &airlineId);
    Q_INVOKABLE void loadTailsigns(const QString &airlineId, const QString &actmodelId);
    Q_INVOKABLE void loadAtaChapters(const QString &airlineId, const QString &actmodelId, const QString &tailsignId);
    Q_INVOKABLE void loadAtaSections(const QString &airlineId, const QString &actmodelId, const QString &tailsignId, const QString &ataChapterId);
    Q_INVOKABLE void loadSwpn(const QString &airlineId, const QString &actmodelId, const QString &tailsignId, const QString &ataChapterId, const QString &ataSectionId);

    QVariant data(const QModelIndex &item, int role) const;
    QHash<int, QByteArray> roleNames() const { return mRoles; }

private:
    void exec(const QString &query);
    QHash<int, QByteArray> mRoles;
};

#endif // DATALOADMODEL_H
