#include "resource.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

Resource::Resource() {
    id = 0;
    consommation = 0;
    prix = 0;
}

Resource::Resource(int id, QString type, QString localisation, double consommation, QString date_mesure, double prix) {
    this->id = id;
    this->type = type;
    this->localisation = localisation;
    this->consommation = consommation;
    this->date_mesure = date_mesure;
    this->prix = prix;
}

// --- Getters ---
int Resource::getId() const { return id; }
QString Resource::getType() const { return type; }
QString Resource::getLocalisation() const { return localisation; }
double Resource::getConsommation() const { return consommation; }
QString Resource::getDateMesure() const { return date_mesure; }
double Resource::getPrix() const { return prix; }

// --- Setters ---
void Resource::setId(int id) { this->id = id; }
void Resource::setType(QString type) { this->type = type; }
void Resource::setLocalisation(QString localisation) { this->localisation = localisation; }
void Resource::setConsommation(double consommation) { this->consommation = consommation; }
void Resource::setDateMesure(QString date_mesure) { this->date_mesure = date_mesure; }
void Resource::setPrix(double prix) { this->prix = prix; }

// --- Ajouter ---
bool Resource::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO RESSOURCES (ID, TYPE, LOCALISATION, CONSOMMATION, DATE_MESURE, PRIX) "
                  "VALUES (:id, :type, :localisation, :consommation, TO_DATE(:date_mesure, 'DD/MM/YYYY'), :prix)");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":localisation", localisation);
    query.bindValue(":consommation", consommation);
    query.bindValue(":date_mesure", date_mesure);
    query.bindValue(":prix", prix);

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

// --- Modifier ---
bool Resource::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCES SET TYPE=:type, LOCALISATION=:localisation, "
                  "CONSOMMATION=:consommation, DATE_MESURE=TO_DATE(:date_mesure, 'DD/MM/YYYY'), "
                  "PRIX=:prix WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":type", type);
    query.bindValue(":localisation", localisation);
    query.bindValue(":consommation", consommation);
    query.bindValue(":date_mesure", date_mesure);
    query.bindValue(":prix", prix);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        qDebug() << "Update error:" << error.text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
