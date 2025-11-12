#include "connection.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;

    if (!c.createConnection()) {
        QMessageBox::critical(nullptr, "Erreur", " Impossible de se connecter à la base de données !");
        return -1;
    } else {
        qDebug() << " Connexion réussie à la base Oracle.";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
