#include "connectdb.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

connectDb::connectDb() {}

bool connectDb::createconnect()
{
    // Configuration de la connexion ODBC
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("QT_Project");  // nom de la source ODBC
    db.setUserName("shimi");           // utilisateur de la base
    db.setPassword("0000");            // mot de passe

    if (!db.open()) {
        qDebug() << "❌ Erreur connexion :" << db.lastError().text();
        return false;
    }

    // ✅ Connexion réussie
    qDebug() << "✅ Connexion à la base réussie !";

    // Vérifier si la table EMPLOYEE est accessible
    QSqlQuery query("SELECT COUNT(*) FROM EMPLOYEE");
    if (query.next()) {
        qDebug() << "📊 Table EMPLOYEE accessible, nombre d'enregistrements :" << query.value(0).toInt();
        return true;
    } else {
        qDebug() << "⚠️ Erreur SQL :" << query.lastError().text();
        return false;
    }
}
