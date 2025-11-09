#include "employee.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructeur avec paramètres
Employee::Employee(const QString &cin, const QString &nom, const QString &prenom,
                   const QString &adresse, const QString &telephone, const QString &metier,
                   double salaire, const QString &presence)
    : m_cin(cin), m_nom(nom), m_prenom(prenom),
    m_adresse(adresse), m_telephone(telephone), m_metier(metier),
    m_salaire(salaire), m_presence(presence)
{
}

// Ajouter un employé dans la table EMPLOYEE
bool Employee::addToDatabase() const
{
    QSqlQuery q;
    q.prepare("INSERT INTO EMPLOYEE (cin, nom, prenom, adresse, telephone, metier, presence, salaire)"
              " VALUES (:cin, :nom, :prenom, :adresse, :telephone, :metier, :presence, :salaire)");
    q.bindValue(":cin", m_cin);
    q.bindValue(":nom", m_nom);
    q.bindValue(":prenom", m_prenom);
    q.bindValue(":adresse", m_adresse);
    q.bindValue(":telephone", m_telephone);
    q.bindValue(":metier", m_metier);
    q.bindValue(":presence", m_presence);
    q.bindValue(":salaire", m_salaire);

    if (!q.exec()) {
        qDebug() << "❌ Erreur SQL (addToDatabase):" << q.lastError().text();
        return false;
    }
    qDebug() << "✅ Employé ajouté :" << m_cin;
    return true;
}

// Mettre à jour un employé
bool Employee::updateInDatabase() const
{
    QSqlQuery q;
    q.prepare("UPDATE EMPLOYEE SET nom=:nom, prenom=:prenom, adresse=:adresse,"
              " telephone=:telephone, metier=:metier, presence=:presence, salaire=:salaire WHERE cin=:cin");
    q.bindValue(":nom", m_nom);
    q.bindValue(":prenom", m_prenom);
    q.bindValue(":adresse", m_adresse);
    q.bindValue(":telephone", m_telephone);
    q.bindValue(":metier", m_metier);
    q.bindValue(":presence", m_presence);
    q.bindValue(":salaire", m_salaire);
    q.bindValue(":cin", m_cin);

    if (!q.exec()) {
        qDebug() << "❌ Erreur SQL (updateInDatabase):" << q.lastError().text();
        return false;
    }
    qDebug() << "✅ Employé mis à jour :" << m_cin;
    return true;
}

// Supprimer un employé par CIN
bool Employee::deleteByCin(const QString &cin)
{
    QSqlQuery q;
    q.prepare("DELETE FROM EMPLOYEE WHERE cin=:cin");
    q.bindValue(":cin", cin);

    if (!q.exec()) {
        qDebug() << "❌ Erreur SQL (deleteByCin):" << q.lastError().text();
        return false;
    }
    qDebug() << "✅ Employé supprimé :" << cin;
    return true;
}

// Récupérer tous les employés
QVector<Employee> Employee::fetchAll()
{
    QVector<Employee> list;
    QSqlQuery q("SELECT cin, nom, prenom, adresse, telephone, metier, presence, salaire FROM EMPLOYEE ORDER BY nom, prenom");

    while (q.next()) {
        // CORRECTION: Respecter l'ordre des paramètres du constructeur
        // (cin, nom, prenom, adresse, telephone, metier, salaire, presence)
        list.append(Employee(q.value(0).toString(),   // cin
                             q.value(1).toString(),   // nom
                             q.value(2).toString(),   // prenom
                             q.value(3).toString(),   // adresse
                             q.value(4).toString(),   // telephone
                             q.value(5).toString(),   // metier
                             q.value(7).toDouble(),   // salaire (position 7)
                             q.value(6).toString())); // presence (position 6)
    }
    return list;
}

// Récupérer un employé par CIN
Employee Employee::fetchByCin(const QString &cin)
{
    QSqlQuery q;
    q.prepare("SELECT cin, nom, prenom, adresse, telephone, metier, presence, salaire FROM EMPLOYEE WHERE cin=:cin");
    q.bindValue(":cin", cin);

    if (q.exec() && q.next()) {
        // CORRECTION: Respecter l'ordre des paramètres
        return Employee(q.value(0).toString(),   // cin
                        q.value(1).toString(),   // nom
                        q.value(2).toString(),   // prenom
                        q.value(3).toString(),   // adresse
                        q.value(4).toString(),   // telephone
                        q.value(5).toString(),   // metier
                        q.value(7).toDouble(),   // salaire (position 7)
                        q.value(6).toString());  // presence (position 6)
    }
    return Employee(); // constructeur par défaut si non trouvé
}

// Récupérer tous les employés triés par salaire (décroissant)
QVector<Employee> Employee::fetchAllSortedBySalaire()
{
    QVector<Employee> list;
    QSqlQuery q("SELECT cin, nom, prenom, adresse, telephone, metier, presence, salaire FROM EMPLOYEE ORDER BY salaire DESC");

    while (q.next()) {
        // CORRECTION: Respecter l'ordre des paramètres
        list.append(Employee(q.value(0).toString(),   // cin
                             q.value(1).toString(),   // nom
                             q.value(2).toString(),   // prenom
                             q.value(3).toString(),   // adresse
                             q.value(4).toString(),   // telephone
                             q.value(5).toString(),   // metier
                             q.value(7).toDouble(),   // salaire (position 7)
                             q.value(6).toString())); // presence (position 6)
    }
    return list;
}

// Convertir un employé en chaîne pour debug
QString Employee::toString() const
{
    return QString("CIN: %1\nNom: %2\nPrénom: %3\nAdresse: %4\nTéléphone: %5\nMétier: %6\nPrésence: %7\nSalaire: %8")
        .arg(m_cin)
        .arg(m_nom)
        .arg(m_prenom)
        .arg(m_adresse)
        .arg(m_telephone)
        .arg(m_metier)
        .arg(m_presence)
        .arg(m_salaire, 0, 'f', 2);
}
