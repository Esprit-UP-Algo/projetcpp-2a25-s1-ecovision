#include "rfid_controller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QSqlRecord>

RFIDController::RFIDController(QObject *parent)
    : QObject(parent)
{
    // Will be set when first RFID card is processed
}

void RFIDController::initializeDatabase()
{
    // Get the active database connection (same one used for login)
    m_database = QSqlDatabase::database();
    if (m_database.isOpen()) {
        qDebug() << "✅ RFIDController using active database connection";
    } else {
        qWarning() << "❌ RFIDController - no active database connection";
    }
}

void RFIDController::procesRfidCard(const QString &rfidUid)
{
    qDebug() << "🔍 Processing RFID card:" << rfidUid;
    
    // Get the active database connection (same one used for login)
    m_database = QSqlDatabase::database();
    
    if (!m_database.isOpen()) {
        qWarning() << "❌ Database not open in RFID Controller";
        qWarning() << "Connection error:" << m_database.lastError().text();
        emit rfidError("Base de données non connectée");
        return;
    }
    
    qDebug() << "✅ Database is open";

    // ✅ CHERCHER LA CARTE DANS Users.uid_rfid
    QSqlQuery query(m_database);
    query.prepare("SELECT cin FROM Users WHERE uid_rfid = :uid");
    query.addBindValue(rfidUid);
    
    if (!query.exec()) {
        qDebug() << "❌ Query failed:" << query.lastError().text();
        emit rfidError("Erreur requête: " + query.lastError().text());
        return;
    }
    
    qDebug() << "✅ Query executed successfully";
    
    // 🔓 CARTE RECONNUE → ACCÈS AUTOMATIQUE
    if (query.next()) {
        QString cinUser = query.value(0).toString();
        qDebug() << "✅ Carte autorisée - CIN:" << cinUser;
        
        // Émettre le signal d'accès autorisé
        emit rfidProcessed(rfidUid, "Accès autorisé - Bienvenue !");
        emit employeeFound(rfidUid, cinUser);
        return;
    }
    
    // ❌ CARTE NON RECONNUE → LOGIN NORMAL
    qDebug() << "❌ Carte non autorisée:" << rfidUid;
    emit rfidNotRecognized(rfidUid);
}
