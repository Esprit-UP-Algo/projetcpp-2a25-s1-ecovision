#ifndef RESIDENT_H
#define RESIDENT_H

#include <QString>
#include <QDate>
#include <QList>

class Resident
{
private:
    // Updated fields for new table structure
    QString cin;              // CIN as primary key
    QString nom;
    QString prenom;
    QString sexe;
    QDate date_naissance;
    QString adresse;
    QString adresse_mail;
    QString telephone;
    QString statut_emploi;    // Job status (Employé/Chômeur/etc.)
    QDate date_inscription;
    QString statut;

public:
    // Default Constructor for fetching data
    Resident();

    // Updated Parameterized constructor with CIN and job status
    Resident(QString cin, QString nom, QString prenom, QString sexe, QDate date_naissance,
             QString adresse, QString adresse_mail, QString telephone, QString statut_emploi);

    // CRUD functions
    bool ajouter();
    static QList<Resident> afficherTous();
    bool modifier();
    bool supprimer();

    // Read functions
    static Resident trouverParCin(QString cin);

    // Getters
    QString getCin() const { return cin; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getSexe() const { return sexe; }
    QDate getDateNaissance() const { return date_naissance; }
    QString getAdresse() const { return adresse; }
    QString getAdresseMail() const { return adresse_mail; }
    QString getTelephone() const { return telephone; }
    QString getStatutEmploi() const { return statut_emploi; }
    QDate getDateInscription() const { return date_inscription; }
    QString getStatut() const { return statut; }

    // Setters
    void setCin(const QString &newCin) { cin = newCin; }
    void setNom(const QString &newNom) { nom = newNom; }
    void setPrenom(const QString &newPrenom) { prenom = newPrenom; }
    void setSexe(const QString &newSexe) { sexe = newSexe; }
    void setDateNaissance(const QDate &newDate) { date_naissance = newDate; }
    void setAdresse(const QString &newAdresse) { adresse = newAdresse; }
    void setAdresseMail(const QString &newAdresseMail) { adresse_mail = newAdresseMail; }
    void setTelephone(const QString &newTelephone) { telephone = newTelephone; }
    void setStatutEmploi(const QString &newStatutEmploi) { statut_emploi = newStatutEmploi; }
    void setStatut(const QString &newStatut) { statut = newStatut; }
};

#endif // RESIDENT_H
