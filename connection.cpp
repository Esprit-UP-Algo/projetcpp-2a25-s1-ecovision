#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

bool Connection::open(QString* err)
{
    // Réutiliser si déjà ouverte
    QSqlDatabase existing = QSqlDatabase::database();
    if (existing.isValid() && existing.isOpen())
        return true;

    // ODBC Oracle via DSN
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QStringLiteral("2A25"));   // DSN
    db.setUserName(QStringLiteral("firas"));
    db.setPassword(QStringLiteral("firas"));

    if (!db.open()) {
        if (err) *err = db.lastError().text();
        return false;
    }

    // Test Oracle correct
    QSqlQuery ping(db);
    if (!ping.exec("SELECT 1 FROM DUAL")) {
        if (err) *err = QStringLiteral("Connexion OK mais test SQL échoué: %1")
                       .arg(ping.lastError().text());
        return false;
    }
    return true;
}
