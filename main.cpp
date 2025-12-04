#include <QApplication>
#include <QMessageBox>

#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Ouverture de la connexion à la base
    QString err;
    if (!Connection::open(&err)) {
        QMessageBox::critical(nullptr,
                              "Connexion ODBC",
                              "Échec d'ouverture de la base via ODBC:\n" + err);
        return 1;
    }

    MainWindow w;
    w.show();

    return app.exec();
}
