#include "local.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QMetaType>
#include <QStringList>

Local::Local(const QString& typeLocal,
             const QString& ville_,
             const QString& adresse_,
             int dispo,
             const QString& nomProp,
             const QString& cinProp,
             const QString& numero_,
             double prix_)
    : type_local(typeLocal)
    , ville(ville_)
    , adresse(adresse_)
    , disponibilite(dispo)
    , nom_proprietaire(nomProp)
    , cin_proprietaire(cinProp)
    , numero(numero_)
    , prix(prix_)
{}

bool Local::isValid(QString* err) const
{
    if (type_local.isEmpty() || ville.isEmpty() || adresse.isEmpty()) {
        if (err) *err = "Type, ville et adresse sont obligatoires.";
        return false;
    }
    if (disponibilite != 0 && disponibilite != 1) {
        if (err) *err = "Disponibilité doit être 0 ou 1.";
        return false;
    }
    if (prix < 0) {
        if (err) *err = "Prix doit être positif.";
        return false;
    }
    if (!cin_proprietaire.trimmed().isEmpty() && cin_proprietaire.size() > 8) {
        if (err) *err = "CIN_PROPRIETAIRE ne doit pas dépasser 8 caractères.";
        return false;
    }
    return true;
}

bool Local::ensureEmployeExists(const QString& cin, QString* err, QSqlDatabase db)
{
    const QString c = cin.trimmed();
    if (c.isEmpty()) return true;

    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();
    QSqlQuery q(useDb);

    q.prepare("SELECT 1 FROM EMPLOYE WHERE CIN = :cin");
    q.bindValue(":cin", c);
    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }
    if (q.next()) return true;

    QSqlQuery ins(useDb);
    ins.prepare("INSERT INTO EMPLOYE (CIN) VALUES (:cin)");
    ins.bindValue(":cin", c);
    if (!ins.exec()) {
        if (err) *err = ins.lastError().text();
        return false;
    }
    return true;
}

bool Local::insert(QString* err, QSqlDatabase db) const
{
    QString fkErr;
    if (!ensureEmployeExists(cin_proprietaire, &fkErr, db)) {
        if (err) *err = fkErr;
        return false;
    }

    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();
    QSqlQuery q(useDb);

    q.prepare(
        "INSERT INTO LOCAL ("
        "  TYPE_LOCAL, VILLE, ADRESSE, DISPONIBILITE,"
        "  NOM_PROPRIETAIRE, CIN_PROPRIETAIRE, NUMERO, PRIX"
        ") VALUES ("
        "  :type_local, :ville, :adresse, :dispo,"
        "  :nom_prop, :cin_prop, :numero, :prix)"
        );

    q.bindValue(":type_local", type_local);
    q.bindValue(":ville",      ville);
    q.bindValue(":adresse",    adresse);
    q.bindValue(":dispo",      disponibilite);
    q.bindValue(":nom_prop",   nom_proprietaire);

    if (cin_proprietaire.trimmed().isEmpty()) {
        const QVariant nullStr(QMetaType::fromType<QString>());
        q.bindValue(":cin_prop", nullStr);
    } else {
        q.bindValue(":cin_prop", cin_proprietaire.trimmed());
    }

    q.bindValue(":numero",     numero);
    q.bindValue(":prix",       prix);

    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }
    return true;
}

bool Local::fetchPrixDispo(int id_local, QVariant* prix, QVariant* dispo, QString* err, QSqlDatabase db)
{
    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();

    QSqlQuery q(useDb);
    q.prepare("SELECT PRIX, DISPONIBILITE FROM LOCAL WHERE ID_LOCAL = :id");
    q.bindValue(":id", id_local);
    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }
    if (!q.next()) {
        if (err) *err = "Aucun local avec cet ID.";
        if (prix)  *prix  = QVariant();
        if (dispo) *dispo = QVariant();
        return false;
    }

    if (prix)  *prix  = q.value(0);
    if (dispo) *dispo = q.value(1);
    return true;
}

bool Local::updateById(int id_local,
                       const std::optional<double>& newPrix,
                       const std::optional<int>& newDispo,
                       QString* err,
                       QSqlDatabase db)
{
    if (!newPrix && !newDispo) {
        if (err) *err = "Rien à modifier.";
        return false;
    }

    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();

    {
        QSqlQuery chk(useDb);
        chk.prepare("SELECT 1 FROM LOCAL WHERE ID_LOCAL = :id");
        chk.bindValue(":id", id_local);
        if (!chk.exec()) {
            if (err) *err = chk.lastError().text();
            return false;
        }
        if (!chk.next()) {
            if (err) *err = "Aucun local avec cet ID.";
            return false;
        }
    }

    QStringList sets;
    if (newPrix)  sets << "PRIX = :prix";
    if (newDispo) sets << "DISPONIBILITE = :dispo";

    QSqlQuery q(useDb);
    q.prepare(QString("UPDATE LOCAL SET %1 WHERE ID_LOCAL = :id").arg(sets.join(", ")));
    if (newPrix)  q.bindValue(":prix",  *newPrix);
    if (newDispo) q.bindValue(":dispo", *newDispo);
    q.bindValue(":id", id_local);

    if (!q.exec()) {
        if (err) *err = q.lastError().text();
        return false;
    }
    return true;
}

bool Local::deleteById(int id_local, QString* err, QSqlDatabase db)
{
    QSqlDatabase useDb = db.isValid() ? db : QSqlDatabase::database();

    {
        QSqlQuery chk(useDb);
        chk.prepare("SELECT 1 FROM LOCAL WHERE ID_LOCAL = :id");
        chk.bindValue(":id", id_local);
        if (!chk.exec()) {
            if (err) *err = chk.lastError().text();
            return false;
        }
        if (!chk.next()) {
            if (err) *err = "Aucun local avec cet ID.";
            return false;
        }
    }

    QSqlQuery del(useDb);
    del.prepare("DELETE FROM LOCAL WHERE ID_LOCAL = :id");
    del.bindValue(":id", id_local);
    if (!del.exec()) {
        if (err) *err = del.lastError().text();
        return false;
    }
    return true;
}
