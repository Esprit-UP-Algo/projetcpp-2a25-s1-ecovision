#include "reclamation.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QSet>

static const char* kTable = "FIRAS.RECLAMATION";  // mets "RECLAMATION" si CURRENT_SCHEMA=FIRAS

Reclamation::Reclamation(int code, const QDate& dateReclam,
                         const QString& nomResident, const QString& description,
                         const QString& etat, const QString& delai)
    : code_(code)
    , dateReclam_(dateReclam)
    , nomResident_(nomResident)
    , description_(description)
    , etat_(etat)
    , delai_(delai)
{}

bool Reclamation::isValid(QString* err) const {
    if (code_ <= 0) { if (err) *err = "CODE doit être > 0."; return false; }
    if (!dateReclam_.isValid()) { if (err) *err = "DATE_RECLAMATION invalide."; return false; }
    if (nomResident_.trimmed().isEmpty()) { if (err) *err = "NOM_RESIDENT est obligatoire."; return false; }
    if (description_.trimmed().isEmpty()) { if (err) *err = "DESCRIPTION est obligatoire."; return false; }

    static const QSet<QString> kAllowed = {"OUVERTE","EN_COURS","CLOSE"};
    if (!kAllowed.contains(etat_)) {
        if (err) *err = "ETAT doit être: OUVERTE, EN_COURS ou CLOSE.";
        return false;
    }
    return true;
}

bool Reclamation::insert(QString* err, QSqlDatabase db) const {
    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();

    QSqlQuery q(useDb);
    q.prepare(QStringLiteral(
                  "INSERT INTO %1 "
                  " (CODE, DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI) "
                  "VALUES "
                  " (:code, :date_reclamation, :nom_resident, :description, :etat, :delai)"
                  ).arg(kTable));

    q.bindValue(":code", code_);
    q.bindValue(":date_reclamation", dateReclam_.startOfDay());   // Qt 6
    q.bindValue(":nom_resident", nomResident_);
    q.bindValue(":description", description_);
    q.bindValue(":etat", etat_);
    q.bindValue(":delai", delai_.trimmed().isEmpty() ? QVariant() : QVariant(delai_.trimmed()));

    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }
    return true;
}
