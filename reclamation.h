#ifndef RECLAMATION_H
#define RECLAMATION_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Reclamation
{
private:
    int code;
    QDate date_reclamation;
    QString nom_resident;
    QString description;
    QString etat;
    QString delai;

public:
    // Constructeurs
    Reclamation();
    Reclamation(int, QDate, QString, QString, QString, QString);

    // Getters
    int getCode() { return code; }
    QDate getDateReclamation() { return date_reclamation; }
    QString getNomResident() { return nom_resident; }
    QString getDescription() { return description; }
    QString getEtat() { return etat; }
    QString getDelai() { return delai; }

    // Setters
    void setCode(int c) { code = c; }
    void setDateReclamation(QDate d) { date_reclamation = d; }
    void setNomResident(QString n) { nom_resident = n; }
    void setDescription(QString desc) { description = desc; }
    void setEtat(QString e) { etat = e; }
    void setDelai(QString del) { delai = del; }

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int);

    // Méthode de recherche
    static Reclamation rechercherParCode(int code);
    static QList<Reclamation> rechercherParDate(QDate date);  // ← NOUVEAU pour le tri
};

#endif // RECLAMATION_H
