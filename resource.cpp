#include "resource.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

Resource::Resource() {
    id = 0;
    consommation = 0;
    prix = 0;
    residentId = 0;  // AJOUTEZ l'initialisation
}

Resource::Resource(int id, QString type, QString localisation, double consommation, QString date_mesure, double prix) {
    this->id = id;
    this->type = type;
    this->localisation = localisation;
    this->consommation = consommation;
    this->date_mesure = date_mesure;
    this->prix = prix;
    this->residentId = 0;  // AJOUTEZ l'initialisation
}
QVector<double> Resource::getConsommationByType() {
    QVector<double> results = {0, 0, 0}; // [water, electricity, other]

    QSqlQuery query;
    query.prepare("SELECT TYPE, SUM(CONSOMMATION) FROM RESSOURCES GROUP BY TYPE");

    if (query.exec()) {
        while (query.next()) {
            QString type = query.value(0).toString().toLower();
            double consommation = query.value(1).toDouble();

            if (type.contains("eau") || type.contains("water")) {
                results[0] = consommation;
            } else if (type.contains("électricité") || type.contains("electricity") || type.contains("elec")) {
                results[1] = consommation;
            } else {
                results[2] += consommation; // Sum other types
            }
        }
    } else {
        QSqlError error = query.lastError();
        qDebug() << "Statistics query error:" << error.text();
    }

    return results;
}
double Resource::getTotalConsommation() {
    QSqlQuery query;
    query.prepare("SELECT SUM(CONSOMMATION) FROM RESSOURCES");

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0;
}

// --- Getters ---
int Resource::getId() const { return id; }
QString Resource::getType() const { return type; }
QString Resource::getLocalisation() const { return localisation; }
double Resource::getConsommation() const { return consommation; }
QString Resource::getDateMesure() const { return date_mesure; }
double Resource::getPrix() const { return prix; }
int Resource::getResidentId() const { return residentId; }  // AJOUTEZ l'implémentation

// --- Setters ---
void Resource::setId(int id) { this->id = id; }
void Resource::setType(const QString &type) { this->type = type; }
void Resource::setLocalisation(const QString &localisation) { this->localisation = localisation; }
void Resource::setConsommation(double consommation) { this->consommation = consommation; }
void Resource::setDateMesure(const QString &date_mesure) { this->date_mesure = date_mesure; }
void Resource::setPrix(double prix) { this->prix = prix; }
void Resource::setResidentId(int id) { this->residentId = id; }  // AJOUTEZ l'implémentation

// --- Ajouter - MODIFIEZ pour inclure RESIDENT_ID ---
bool Resource::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO RESSOURCES (ID, TYPE, LOCALISATION, CONSOMMATION, DATE_MESURE, PRIX, RESIDENT_ID) "
                  "VALUES (:id, :type, :localisation, :consommation, TO_DATE(:date_mesure, 'DD/MM/YYYY'), :prix, :resident_id)");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":localisation", localisation);
    query.bindValue(":consommation", consommation);
    query.bindValue(":date_mesure", date_mesure);
    query.bindValue(":prix", prix);
    query.bindValue(":resident_id", residentId);  // AJOUTEZ

    if (!query.exec()) {
        QSqlError error = query.lastError();
        qDebug() << "Insert error:" << error.text();
        qDebug() << "SQL:" << query.lastQuery();
        return false;
    }

    return true;
}

// --- Supprimer ---
bool Resource::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM RESSOURCES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        qDebug() << "Delete error:" << error.text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

// --- Modifier - MODIFIEZ pour inclure RESIDENT_ID ---
bool Resource::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCES SET TYPE=:type, LOCALISATION=:localisation, "
                  "CONSOMMATION=:consommation, DATE_MESURE=TO_DATE(:date_mesure, 'DD/MM/YYYY'), "
                  "PRIX=:prix, RESIDENT_ID=:resident_id WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":localisation", localisation);
    query.bindValue(":consommation", consommation);
    query.bindValue(":date_mesure", date_mesure);
    query.bindValue(":prix", prix);
    query.bindValue(":resident_id", residentId);  // AJOUTEZ

    if (!query.exec()) {
        QSqlError error = query.lastError();
        qDebug() << "Update error:" << error.text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
