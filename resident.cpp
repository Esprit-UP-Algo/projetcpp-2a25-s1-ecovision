#include "resident.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

// Default constructor
Resident::Resident() :
    cin(""),
    nom(""),
    prenom(""),
    sexe(""),
    adresse(""),
    adresse_mail(""),
    telephone(""),
    statut_emploi(""),
    statut("Actif")
{
    date_inscription = QDate::currentDate();
}

// Updated Parameterized constructor with CIN and job status
Resident::Resident(QString cin, QString nom, QString prenom, QString sexe, QDate date_naissance,
                   QString adresse, QString adresse_mail, QString telephone, QString statut_emploi) :
    cin(cin),
    nom(nom),
    prenom(prenom),
    sexe(sexe),
    date_naissance(date_naissance),
    adresse(adresse),
    adresse_mail(adresse_mail),
    telephone(telephone),
    statut_emploi(statut_emploi),
    statut("Actif")
{
    date_inscription = QDate::currentDate();
}

bool Resident::ajouter()
{
    QSqlQuery query;

    // Check if CIN already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM RESIDENT WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            qDebug() << "❌ CIN already exists:" << cin;
            return false;
        }
    }

    query.prepare("INSERT INTO RESIDENT (CIN, NOM, PRENOM, SEXE, DATE_NAISSANCE, "
                  "ADRESSE, ADRESSE_MAIL, TELEPHONE, STATUT_EMPLOI, DATE_INSCRIPTION, STATUT) "
                  "VALUES (:cin, :nom, :prenom, :sexe, :date_naissance, "
                  ":adresse, :adresse_mail, :telephone, :statut_emploi, :date_inscription, :statut)");

    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":adresse_mail", adresse_mail);
    query.bindValue(":telephone", telephone);
    query.bindValue(":statut_emploi", statut_emploi);
    query.bindValue(":date_inscription", date_inscription);
    query.bindValue(":statut", statut);

    if (query.exec()) {
        qDebug() << "✅ Resident added successfully! CIN:" << cin;
        return true;
    } else {
        qDebug() << "❌ Error adding resident:" << query.lastError().text();
        return false;
    }
}

bool Resident::supprimer()
{
    if (cin.isEmpty()) {
        qDebug() << "❌ Cannot delete resident without CIN";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM RESIDENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        qDebug() << "✅ Resident deleted successfully! CIN:" << cin;
        return true;
    } else {
        qDebug() << "❌ Error deleting resident:" << query.lastError().text();
        return false;
    }
}

bool Resident::modifier()
{
    if (cin.isEmpty()) {
        qDebug() << "❌ Cannot update resident without CIN";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE RESIDENT SET "
                  "NOM = :nom, PRENOM = :prenom, SEXE = :sexe, DATE_NAISSANCE = :date_naissance, "
                  "ADRESSE = :adresse, ADRESSE_MAIL = :adresse_mail, "
                  "TELEPHONE = :telephone, STATUT_EMPLOI = :statut_emploi, STATUT = :statut "
                  "WHERE CIN = :cin");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":sexe", sexe);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":adresse_mail", adresse_mail);
    query.bindValue(":telephone", telephone);
    query.bindValue(":statut_emploi", statut_emploi);
    query.bindValue(":statut", statut);
    query.bindValue(":cin", cin);

    if (query.exec()) {
        qDebug() << "✅ Resident updated successfully! CIN:" << cin;
        return true;
    } else {
        qDebug() << "❌ Error updating resident:" << query.lastError().text();
        return false;
    }
}

QList<Resident> Resident::afficherTous()
{
    QList<Resident> residents;
    QSqlQuery query("SELECT * FROM RESIDENT ORDER BY CIN");

    while (query.next()) {
        Resident resident;
        resident.setCin(query.value("CIN").toString());
        resident.setNom(query.value("NOM").toString());
        resident.setPrenom(query.value("PRENOM").toString());
        resident.setSexe(query.value("SEXE").toString());
        resident.setDateNaissance(query.value("DATE_NAISSANCE").toDate());
        resident.setAdresse(query.value("ADRESSE").toString());
        resident.setAdresseMail(query.value("ADRESSE_MAIL").toString());
        resident.setTelephone(query.value("TELEPHONE").toString());
        resident.setStatutEmploi(query.value("STATUT_EMPLOI").toString());
        resident.setStatut(query.value("STATUT").toString());

        residents.append(resident);
    }

    return residents;
}

Resident Resident::trouverParCin(QString cin)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM RESIDENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    Resident resident;

    if (query.exec() && query.next()) {
        resident.setCin(query.value("CIN").toString());
        resident.setNom(query.value("NOM").toString());
        resident.setPrenom(query.value("PRENOM").toString());
        resident.setSexe(query.value("SEXE").toString());
        resident.setDateNaissance(query.value("DATE_NAISSANCE").toDate());
        resident.setAdresse(query.value("ADRESSE").toString());
        resident.setAdresseMail(query.value("ADRESSE_MAIL").toString());
        resident.setTelephone(query.value("TELEPHONE").toString());
        resident.setStatutEmploi(query.value("STATUT_EMPLOI").toString());
        resident.setStatut(query.value("STATUT").toString());
    }

    return resident;
}
