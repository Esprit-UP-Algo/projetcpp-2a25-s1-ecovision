#ifndef LOCAL_H
#define LOCAL_H

#include <QString>

class Local
{
private:
    int id_local;
    QString type_local;
    QString ville;
    QString adresse;
    int disponibilite;
    QString nom_proprietaire;
    QString cin_proprietaire;
    QString numero_tel;
    double prix;

public:
    Local();
    Local(int, QString, QString, QString, int, QString, QString, QString, double);

    // Getters
    int getIdLocal() const;
    QString getTypeLocal() const;
    QString getVille() const;
    QString getAdresse() const;
    int getDisponibilite() const;
    QString getNomProprietaire() const;
    QString getCinProprietaire() const;
    QString getNumeroTel() const;
    double getPrix() const;

    // CRUD Operations
    bool ajouter();
    bool modifier();
    static Local* rechercherParId(int id);
};

#endif // LOCAL_H
