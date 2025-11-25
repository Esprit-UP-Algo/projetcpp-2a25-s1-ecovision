#ifndef RFID_CONTROLLER_H
#define RFID_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class RFIDController : public QObject
{
    Q_OBJECT

public:
    explicit RFIDController(QObject *parent = nullptr);
    
    // Process RFID card scan and record to database
    void procesRfidCard(const QString &rfidUid);
    
    // Initialiser/réinitialiser la connexion DB
    void initializeDatabase();
    
signals:
    void rfidProcessed(const QString &uid, const QString &message);
    void rfidError(const QString &error);
    void employeeFound(const QString &uid, const QString &cin);
    void rfidNotRecognized(const QString &uid);

private:
    QSqlDatabase m_database;
};

#endif // RFID_CONTROLLER_H
