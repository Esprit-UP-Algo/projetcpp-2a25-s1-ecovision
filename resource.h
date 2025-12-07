#ifndef RESOURCE_H
#define RESOURCE_H

#include <QString>

class Resource
{
private:
    int id;
    QString type;
    QString localisation;
    double consommation;
    QString date_mesure;
    double prix;
    int residentId;

public:
    Resource();
    Resource(int, QString, QString, double, QString, double);
    static QVector<double> getConsommationByType();
    static QVector<double> getMonthlyConsommation(int year = -1);
    static QVector<double> getConsommationByLocation();
    static double getTotalConsommation();


    // Getters
    int getId() const;
    QString getType() const;
    QString getLocalisation() const;
    double getConsommation() const;
    QString getDateMesure() const;
    double getPrix() const;
    int getResidentId() const;  // CORRIGÉ: retirez getDefaultResidentId()

    // Setters
    void setId(int);
    void setType(const QString &);
    void setLocalisation(const QString &);
    void setConsommation(double);
    void setDateMesure(const QString &);
    void setPrix(double);
    void setResidentId(int);  // CORRIGÉ: séparez la déclaration

    // CRUD
    bool ajouter();
    bool supprimer(int);
    bool modifier();
};

#endif // RESOURCE_H
