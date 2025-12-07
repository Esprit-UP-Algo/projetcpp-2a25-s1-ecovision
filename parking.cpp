#include "parking.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QDebug>

// Constructeur
Parking::Parking(int id, QString nom, QString emplacement, int capacite, int placesDisponibles, double tarifHoraire, QString statut)
    : id(id), nom(nom), emplacement(emplacement), capacite(capacite),
    placesDisponibles(placesDisponibles), tarifHoraire(tarifHoraire), statut(statut) {}

// Ajouter un parking
bool Parking::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO PARKING (ID, NOM, EMPLACEMENT, CAPACITE, PLACES_DISPONIBLES, TARIF_HORAIRE, STATUT) "
                  "VALUES (:id, :nom, :emplacement, :capacite, :placesDisponibles, :tarifHoraire, :statut)");
    query.bindValue(":id", id); //bindvalue pour eviter injecion sql
    query.bindValue(":nom", nom);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placesDisponibles", placesDisponibles);
    query.bindValue(":tarifHoraire", tarifHoraire);
    query.bindValue(":statut", statut);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du parking:" << query.lastError().text();
        qDebug() << "Executed Query:" << query.lastQuery();
        qDebug() << "Bound Values:" << query.boundValues();
        return false;
    }
    return true;
}

// Mettre à jour un parking existant
bool Parking::mettreAJour(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE PARKING SET NOM = :nom, EMPLACEMENT = :emplacement, CAPACITE = :capacite, "
                  "PLACES_DISPONIBLES = :placesDisponibles, TARIF_HORAIRE = :tarifHoraire, STATUT = :statut "
                  "WHERE ID = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":emplacement", emplacement);
    query.bindValue(":capacite", capacite);
    query.bindValue(":placesDisponibles", placesDisponibles);
    query.bindValue(":tarifHoraire", tarifHoraire);
    query.bindValue(":statut", statut);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la mise à jour du parking:" << query.lastError().text();
        return false;
    }
    return true;
}

// Supprimer un parking
bool Parking::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PARKING WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du parking:" << query.lastError().text();
        return false;
    }
    return true;
}

// Vérifier l'existence d'un parking
bool Parking::exists(int id)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PARKING WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification d'existence:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}

// Afficher tous les parkings
QSqlQueryModel* Parking::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID, NOM, EMPLACEMENT, CAPACITE, PLACES_DISPONIBLES, TARIF_HORAIRE, STATUT FROM PARKING");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places disponibles"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Tarif horaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}

QSqlQueryModel* Parking::trierParId()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PARKING ORDER BY ID ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places disponibles"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Tarif horaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
QSqlQueryModel* Parking::trierParNom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PARKING ORDER BY NOM ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places disponibles"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Tarif horaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
QSqlQueryModel* Parking::trierParCapacite()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PARKING ORDER BY CAPACITE ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places disponibles"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Tarif horaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
QSqlQueryModel* Parking::rechercher(int id)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM PARKING WHERE ID = :id");
    query.bindValue(":id", id);
    query.exec();

    model->setQuery(query);

    // Mettre les en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Emplacement"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Places disponibles"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Tarif horaire"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}


