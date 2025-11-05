#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>

bool Connection::open(QString* err)
{
    // Réutilise une connexion déjà ouverte
    QSqlDatabase existing = QSqlDatabase::database();
    if (existing.isValid() && existing.isOpen())
        return true;

    // Driver ODBC, DSN “2A25”, user “firas”, password “firas”
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QStringLiteral("2A25"));
    db.setUserName(QStringLiteral("firas"));
    db.setPassword(QStringLiteral("firas"));

    if (!db.open()) {
        if (err) *err = db.lastError().text();
        return false;
    }
    return true;
}
