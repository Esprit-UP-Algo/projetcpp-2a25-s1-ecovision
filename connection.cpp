#include "connection.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>


Connection::Connection()
{
}

bool Connection::createConnection()
{
    bool test = false;

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("projet_2A2526");
    db.setUserName("dhia");
    db.setPassword("dhia123");            // Mot de passe Oracle

    if (db.open()) {
        test = true;
        qDebug() << "✅ Connexion réussie à la base Oracle.";
    } else {
        qDebug() << "❌ Échec de connexion à la base Oracle:" << db.lastError().text();
    }

    return test;
}

void Connection::closeConnection()
{
    if (db.isOpen())
        db.close();
}
