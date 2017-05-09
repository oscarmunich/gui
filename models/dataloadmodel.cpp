#include "dataloadmodel.h"

DataLoadModel::DataLoadModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    //QHash<int, QByteArray> roles;
    mRoles.insert(IdRole, "id");
    mRoles.insert(ValueRole, "value");
    //setRoleNames(roles);
}

void DataLoadModel::resetModel()
{
    exec("");
}

void DataLoadModel::loadAirlines()
{
    exec("SELECT a.iata_code AS airline_id, a.airline AS airline_displayed FROM vw_pdl_airline a ORDER BY airline_displayed");
}

void DataLoadModel::loadActmodels(const QString &airlineId)
{
    exec(QString("SELECT DISTINCT ac.acmodel AS acmodel_id, ac.acmodel AS acmodel_displayed FROM vw_pdl_swpn_ac sa "
                 "JOIN vw_pdl_ac ac ON (sa.tailsign=ac.tailsign) "
                 "WHERE ac.iata_code='%1' "
                 "ORDER BY acmodel_displayed").arg(airlineId));
}

void DataLoadModel::loadTailsigns(const QString &airlineId, const QString &actmodelId)
{
    exec(QString("SELECT DISTINCT sa.tailsign AS tailsign_id, sa.tailsign AS tailsign_displayed FROM vw_pdl_swpn_ac sa "
                 "JOIN vw_pdl_ac ac ON (sa.tailsign=ac.tailsign) "
                 "WHERE ac.iata_code='%1' AND ac.acmodel='%2' "
                 "ORDER BY tailsign_displayed").arg(airlineId).arg(actmodelId));
}

void DataLoadModel::loadAtaChapters(const QString &airlineId, const QString &actmodelId, const QString &tailsignId)
{
    exec(QString("SELECT DISTINCT achap.atachapter_no AS atachapter_id, achap.atachapter_no || ' - ' || achap.atachapter AS atachapter_displayed FROM vw_pdl_swpn_ac sa "
                 "JOIN vw_pdl_ac ac ON (sa.tailsign=ac.tailsign) "
                 "JOIN vw_pdl_atasection asec ON (sa.atasection_no=asec.atasection_no AND sa.atachapter_no=asec.atachapter_no AND ac.acmodel=asec.acmodel) "
                 "JOIN vw_pdl_atachapter achap ON (asec.atachapter_no=achap.atachapter_no) "
                 "WHERE ac.iata_code='%1' AND ac.acmodel='%2' AND ac.tailsign='%3' "
                 "ORDER BY atachapter_displayed").arg(airlineId).arg(actmodelId).arg(tailsignId));
}

void DataLoadModel::loadAtaSections(const QString &airlineId, const QString &actmodelId, const QString &tailsignId, const QString &ataChapterId)
{
    exec(QString("SELECT DISTINCT asec.atasection_no AS atasection_id, asec.atasection_no || ' - ' || asec.atasection AS atasection_displayed FROM vw_pdl_swpn_ac sa "
                 "JOIN vw_pdl_ac ac ON (sa.tailsign=ac.tailsign) "
                 "JOIN vw_pdl_atasection asec ON (sa.atasection_no=asec.atasection_no AND sa.atachapter_no=asec.atachapter_no AND ac.acmodel=asec.acmodel) "
                 "JOIN vw_pdl_atachapter achap ON (asec.atachapter_no=achap.atachapter_no) "
                 "WHERE ac.iata_code='%1' AND ac.acmodel='%2' AND ac.tailsign='%3' AND asec.atachapter_no='%4' "
                 "ORDER BY atasection_displayed").arg(airlineId).arg(actmodelId).arg(tailsignId).arg(ataChapterId));
}

void DataLoadModel::loadSwpn(const QString &airlineId, const QString &actmodelId, const QString &tailsignId, const QString &ataChapterId, const QString &ataSectionId)
{
    exec(QString("SELECT DISTINCT s.swpn AS swpn_id, s.swpn || ' - ' || s.title AS swpn_displayed FROM vw_pdl_swpn_ac sa "
                 "JOIN vw_pdl_ac ac ON (sa.tailsign=ac.tailsign) "
                 "JOIN vw_pdl_atasection asec ON (sa.atasection_no=asec.atasection_no AND sa.atachapter_no=asec.atachapter_no AND ac.acmodel=asec.acmodel) "
                 "JOIN vw_pdl_swpn s ON (sa.swpn=s.swpn) "
                 "WHERE ac.iata_code='%1' AND ac.acmodel='%2' AND ac.tailsign='%3' "
                 "AND asec.atachapter_no='%4' AND asec.atasection_no='%5' "
                 "ORDER BY swpn_displayed;").arg(airlineId).arg(actmodelId).arg(tailsignId).arg(ataChapterId).arg(ataSectionId));
}

QVariant DataLoadModel::data(const QModelIndex &item, int role) const
{
    if (!item.isValid())
        return QVariant();

    switch (role) {
    case IdRole:
        return QSqlQueryModel::data(QSqlQueryModel::index(item.row(), 0));
        break;

    case ValueRole:
        return QSqlQueryModel::data(QSqlQueryModel::index(item.row(), 1));
        break;
    }

    return QVariant();
}

void DataLoadModel::exec(const QString &query)
{
    setQuery("");
    setQuery(query);
}
