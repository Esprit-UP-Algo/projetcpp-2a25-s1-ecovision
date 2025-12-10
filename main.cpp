#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QFontDatabase>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load font (Keep this section if needed)
    int fontId = QFontDatabase::addApplicationFont(":/fonts/roboto.ttf");
    if (fontId != -1) {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont appFont(family, 11);
        a.setFont(appFont);
    }

    // Database Connection
    Connection conn;

    if (!conn.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur de connexion",
                              "Impossible de se connecter à la base de données. L'application va se fermer.");
        return 1; // Exit application if connection fails
    }

    // Application Start
    MainWindow w;
    w.show();
    return a.exec();
}
