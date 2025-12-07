#ifndef PARKING_H
#define PARKING_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Parking
{
    int id;
    QString nom;
    QString emplacement;
    int capacite;
    int placesDisponibles;
    double tarifHoraire;
    QString statut; // ex: "ouvert", "fermé", "complet"

public:
    // Constructeurs
    Parking() {};
    Parking(int, QString, QString, int, int, double, QString);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getEmplacement() const { return emplacement; }
    int getCapacite() const { return capacite; }
    int getPlacesDisponibles() const { return placesDisponibles; }
    double getTarifHoraire() const { return tarifHoraire; }
    QString getStatut() const { return statut; }

    // Setters
    void setId(int i) { id = i; }
    void setNom(const QString& n) { nom = n; }
    void setEmplacement(const QString& e) { emplacement = e; }
    void setCapacite(int c) { capacite = c; }
    void setPlacesDisponibles(int p) { placesDisponibles = p; }
    void setTarifHoraire(double t) { tarifHoraire = t; }
    void setStatut(const QString& s) { statut = s; }

    // Database Functions
    bool ajouter();
    bool mettreAJour(int id);
    bool supprimer(int id);
    bool exists(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* trierParId();
    QSqlQueryModel* trierParNom();
    QSqlQueryModel* trierParCapacite();
    QSqlQueryModel* rechercher(int id);



};

#endif // PARKING_H
