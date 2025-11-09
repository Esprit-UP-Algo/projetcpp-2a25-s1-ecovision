#pragma once
#include <QString>
#include <QVector>

class Employee
{
public:
    // Constructeur par défaut
    Employee() = default;

    // Constructeur principal
    Employee(const QString &cin, const QString &nom, const QString &prenom,
             const QString &adresse, const QString &telephone, const QString &metier,
             double salaire = 0.0, const QString &presence = "");

    // Méthodes SQL
    bool addToDatabase() const;
    bool updateInDatabase() const;
    static bool deleteByCin(const QString &cin);
    static QVector<Employee> fetchAll();
    static QVector<Employee> fetchAllSortedBySalaire();  // ✅ ajout nécessaire ici
    static Employee fetchByCin(const QString &cin);

    // Accesseurs
    QString getCin() const { return m_cin; }
    QString getNom() const { return m_nom; }
    QString getPrenom() const { return m_prenom; }
    QString getAdresse() const { return m_adresse; }
    QString getTelephone() const { return m_telephone; }
    QString getMetier() const { return m_metier; }
    double getSalaire() const { return m_salaire; }
    QString getPresence() const { return m_presence; }

    QString toString() const;

private:
    QString m_cin;
    QString m_nom;
    QString m_prenom;
    QString m_adresse;
    QString m_telephone;
    QString m_metier;
    double m_salaire = 0.0;
    QString m_presence;
};
