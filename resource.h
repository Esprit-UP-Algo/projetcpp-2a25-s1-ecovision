#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Resource
{
private:
    int id;
    QString type;
    QString localisation;
    double consommation;
    QString date_mesure;
    double prix;

public:
    Resource();
    Resource(int, QString, QString, double, QString, double);

    // Getters
    int getId() const;
    QString getType() const;
    QString getLocalisation() const;
    double getConsommation() const;
    QString getDateMesure() const;
    double getPrix() const;

    // Setters
    void setId(int);
    void setType(QString);
    void setLocalisation(QString);
    void setConsommation(double);
    void setDateMesure(QString);
    void setPrix(double);

    // CRUD
    bool ajouter();
    bool supprimer(int);
    bool modifier();
};

#endif // RESOURCE_H
