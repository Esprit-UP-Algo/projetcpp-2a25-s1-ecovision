#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

class Connection
{
public:
    Connection() {}

    /**
     * @brief Attempts to establish a connection to the Oracle database using the QODBC driver.
     * @return true if the connection is successful, false otherwise.
     */
    bool createConnection()
    {
        // 1. Revert to QODBC driver, as QOCI is unavailable due to installation constraints.
        // We use the default connection name ("qt_sql_default_connection").
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");

        // 2. Set parameters to match your previously working credentials and the DSN name.
        db.setDatabaseName("OracleXE"); // This must match the System DSN name on your machine
        db.setUserName("SYSTEM");       // Your schema username (based on the password you provided)
        db.setPassword("09403098");     // Your password

        qDebug() << "Testing QODBC connection to OracleXE...";

        if (!db.open()) {
            qDebug() << "❌ Connection failed (QODBC):" << db.lastError().text();

            QMessageBox::critical(nullptr, QObject::tr("Database Connection Error"),
                                  QObject::tr("Failed to connect via QODBC. Ensure the 'OracleXE' DSN is correctly configured and the Oracle service is running."),
                                  QMessageBox::Cancel);
            return false;
        }

        qDebug() << "✅ QODBC connection successful!";

        // Run a simple test query to confirm full database functionality
        testConnection();

        return true;
    }

    /**
     * @brief Closes the connection and removes the database instance.
     */
    void closeConnection()
    {
        // Get the default connection
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen()) {
            db.close();
            QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        }
    }

    /**
     * @brief Returns the active database connection.
     * @return The QSqlDatabase instance.
     */
    QSqlDatabase getDatabase()
    {
        return QSqlDatabase::database();
    }

    /**
     * @brief Executes a simple query to verify the connection is live.
     * @return true if the test query succeeds, false otherwise.
     */
    bool testConnection()
    {
        QSqlQuery query(getDatabase());
        if (query.exec("SELECT 'Connection successful' FROM dual")) {
            if (query.next()) {
                qDebug() << "Test result:" << query.value(0).toString();
            }
            return true;
        } else {
            qDebug() << "Test failed:" << query.lastError().text();
            return false;
        }
    }
};

#endif // CONNECTION_H
