// connection.cpp
#include "connection.h"
#include "ui_connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

connection::connection(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::connection)
{
    ui->setupUi(this);
    // login button: pushButton_2
    connect(ui->pushButton_2, &QPushButton::clicked, this, &connection::onLoginClicked);
}

connection::~connection()
{
    delete ui;
}

void connection::onLoginClicked()
{
    const QString cin = ui->CIN_2->text().trimmed();
    const QString mdp = ui->mdp_2->text().trimmed();

    if (cin.length() != 8) {
        QMessageBox::warning(this, tr("Connexion"), tr("CIN invalide (8 caractères)."));
        return;
    }
    if (mdp.isEmpty()) {
        QMessageBox::warning(this, tr("Connexion"), tr("Mot de passe requis."));
        return;
    }

    // Auth RH: Users table with mdp
    QSqlQuery q;
    q.prepare("SELECT 1 FROM Users WHERE cin = :cin AND mdp = :mdp");
    q.bindValue(":cin", cin);
    q.bindValue(":mdp", mdp);
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Erreur SQL"), q.lastError().text());
        return;
    }
    if (q.next()) {
        emit loginSucceeded();
    } else {
        QMessageBox::warning(this, tr("Connexion"), tr("CIN ou mot de passe invalide."));
    }
}
