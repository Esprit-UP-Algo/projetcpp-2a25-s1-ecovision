#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "connectdb.h"
#include "arduino_manager.h"
#include "rfid_controller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    connection c;
    connectDb db;
    bool test=db.createconnect();
    if(!test)
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

    // ✅ Initialize RFID system IMMEDIATELY (before login)
    ArduinoManager* arduinoManager = new ArduinoManager();
    RFIDController* rfidController = new RFIDController();
    // Ouvrir le port Arduino pour écouter les scans pendant le login
    const QString arduinoPort = "COM10"; // Adapter si différent
    if (!arduinoManager->open(arduinoPort, 9600)) {
        qWarning() << "❌ Impossible d'ouvrir le port" << arduinoPort << ":";
        QMessageBox::warning(nullptr, "Arduino", QString("Arduino non connecté sur %1. Scan RFID inactif.").arg(arduinoPort));
    } else {
        qDebug() << "✅ RFID actif sur" << arduinoPort << "(phase login)";
    }
    
    // When RFID card is scanned → process it
    QObject::connect(arduinoManager, &ArduinoManager::rfidScanned, [&](const QString &uid) {
        qDebug() << "📡 RFID card detected during login:" << uid;
        rfidController->procesRfidCard(uid);
    });
    
    // When card is authorized → open employee management directly (bypass login)
    QObject::connect(rfidController, &RFIDController::employeeFound, [&](const QString &uid, const QString &cin) {
        qDebug() << "✅ Carte autorisée - Accès direct à la gestion";
        // Fermer l'instance initiale avant que MainWindow crée la sienne
        if (arduinoManager->isOpen()) {
            arduinoManager->close();
        }
        w.initializeRFID(); // crée sa propre instance interne pour usage post-login
        w.show();
        w.onDisplayAllEmployes();
        c.close();  // Close login dialog
    });
    
    // When card is NOT authorized → show message
    QObject::connect(rfidController, &RFIDController::rfidNotRecognized, [&](const QString &uid) {
        qDebug() << "❌ Carte non autorisée - Affichage du login";
        QMessageBox::warning(nullptr, "Accès refusé", 
                           "Cette carte n'est pas autorisée. Veuillez vous connecter avec CIN + MDP.");
    });
    
    // Normal login succeeded via cin + mdp
    QObject::connect(&c, &connection::loginSucceeded, [&]() {
        w.initializeRFID();
        w.show();
        w.onDisplayAllEmployes();
        c.close();
    });

    // ✅ Show login interface at startup
    c.show();
    
    qDebug() << "✅ Application démarrée - Interface de login affichée";
    qDebug() << "🔍 RFID actif - En attente: CIN+MDP ou scan RFID...";
    
    return a.exec();
}
