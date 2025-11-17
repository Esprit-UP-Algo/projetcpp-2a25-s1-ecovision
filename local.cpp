#include "local.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Local::Local() {
    id_local = 0;
    disponibilite = 1;
    prix = 0.0;
}

Local::Local(int id, QString type, QString ville, QString adresse, int dispo,
             QString nomProp, QString cin, QString tel, double prix) {
    this->id_local = id;
    this->type_local = type;
    this->ville = ville;
    this->adresse = adresse;
    this->disponibilite = dispo;
    this->nom_proprietaire = nomProp;
    this->cin_proprietaire = cin;
    this->numero_tel = tel;
    this->prix = prix;
}

// Getters
int Local::getIdLocal() const { return id_local; }
QString Local::getTypeLocal() const { return type_local; }
QString Local::getVille() const { return ville; }
QString Local::getAdresse() const { return adresse; }
int Local::getDisponibilite() const { return disponibilite; }
QString Local::getNomProprietaire() const { return nom_proprietaire; }
QString Local::getCinProprietaire() const { return cin_proprietaire; }
QString Local::getNumeroTel() const { return numero_tel; }
double Local::getPrix() const { return prix; }

bool Local::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO LOCAUX (ID_LOCAL, TYPE_LOCAL, VILLE, ADRESSE, DISPONIBILITE, "
                  "NOM_PROPRIETAIRE, CIN_PROPRIETAIRE, NUMERO_TEL, PRIX) "
                  "VALUES (:id, :type, :ville, :adresse, :dispo, :nom, :cin, :tel, :prix)");

    query.bindValue(":id", id_local);
    query.bindValue(":type", type_local);
    query.bindValue(":ville", ville);
    query.bindValue(":adresse", adresse);
    query.bindValue(":dispo", disponibilite);
    query.bindValue(":nom", nom_proprietaire);
    query.bindValue(":cin", cin_proprietaire);
    query.bindValue(":tel", numero_tel);
    query.bindValue(":prix", prix);

    if (!query.exec()) {
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Local::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE LOCAUX SET TYPE_LOCAL=:type, VILLE=:ville, ADRESSE=:adresse, "
                  "DISPONIBILITE=:dispo, NOM_PROPRIETAIRE=:nom, CIN_PROPRIETAIRE=:cin, "
                  "NUMERO_TEL=:tel, PRIX=:prix WHERE ID_LOCAL=:id");

    query.bindValue(":id", id_local);
    query.bindValue(":type", type_local);
    query.bindValue(":ville", ville);
    query.bindValue(":adresse", adresse);
    query.bindValue(":dispo", disponibilite);
    query.bindValue(":nom", nom_proprietaire);
    query.bindValue(":cin", cin_proprietaire);
    query.bindValue(":tel", numero_tel);
    query.bindValue(":prix", prix);

    if (!query.exec()) {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }
    return true;
}

Local* Local::rechercherParId(int id) {
    QSqlQuery query;
    query.prepare("SELECT * FROM LOCAUX WHERE ID_LOCAL = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return new Local(
            query.value("ID_LOCAL").toInt(),
            query.value("TYPE_LOCAL").toString(),
            query.value("VILLE").toString(),
            query.value("ADRESSE").toString(),
            query.value("DISPONIBILITE").toInt(),
            query.value("NOM_PROPRIETAIRE").toString(),
            query.value("CIN_PROPRIETAIRE").toString(),
            query.value("NUMERO_TEL").toString(),
            query.value("PRIX").toDouble()
            );
    }
    return nullptr;
}
