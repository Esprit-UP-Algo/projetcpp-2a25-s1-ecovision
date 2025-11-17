#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Create and test database connection
    Connection c;

    if (!c.createConnection()) {  // ✅ Match the correct method name
        QMessageBox::critical(nullptr,
                              "Erreur de connexion",
                              "Impossible de se connecter à la base de données Oracle.\n"
                              "Veuillez vérifier:\n"
                              "- Que le service Oracle est démarré\n"
                              "- Que les paramètres ODBC sont configurés (projet_2A2526)\n"
                              "- Que l'utilisateur 'dhia' existe avec le mot de passe correct\n\n"
                              "Erreur détaillée dans la console de débogage.");
        qDebug() << "❌ Database connection failed - check ODBC configuration";
        return -1;
    }

    qDebug() << "✅ Database connection established successfully via ODBC";

    // Create and show main window
    MainWindow w;
    w.show();

    // Close connection when application exits
    QObject::connect(&a, &QApplication::aboutToQuit, [&c]() {
        c.closeConnection();
        qDebug() << "Database connection closed";
    });

    return a.exec();
}
