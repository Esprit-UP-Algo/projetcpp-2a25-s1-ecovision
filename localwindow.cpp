#include "localwindow.h"
#include "ui_localwindow.h"
#include "local.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDebug>

LocalWindow::LocalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    model(new QSqlQueryModel(this))
{
    ui->setupUi(this);
    setWindowTitle("Gestion des Locaux");


    createLocalTable();
    afficherTable();
}

LocalWindow::~LocalWindow()
{
    delete ui;
}

bool LocalWindow::createLocalTable()
{
    QSqlQuery query;
    QString createTableSQL =
        "CREATE TABLE IF NOT EXISTS LOCAUX ("
        "   ID_LOCAL INTEGER PRIMARY KEY, "
        "   TYPE_LOCAL VARCHAR(100), "
        "   VILLE VARCHAR(100), "
        "   ADRESSE VARCHAR(200), "
        "   DISPONIBILITE INTEGER, "
        "   NOM_PROPRIETAIRE VARCHAR(100), "
        "   CIN_PROPRIETAIRE VARCHAR(20), "
        "   NUMERO_TEL VARCHAR(20), "
        "   PRIX REAL"
        ")";

    if (!query.exec(createTableSQL)) {
        qDebug() << "Erreur création table:" << query.lastError().text();
        return false;
    }
    return true;
}

void LocalWindow::afficherTable()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM LOCAUX ORDER BY ID_LOCAL");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur chargement données: " + query.lastError().text());
        return;
    }

    // CORRECTION: Utiliser std::move
    model->setQuery(std::move(query));
    ui->tableView->setModel(model);
}

void LocalWindow::on_ajouterButton_clicked()
{
    if (ui->l1->text().isEmpty() || ui->l2->text().isEmpty() || ui->l3->text().isEmpty() ||
        ui->l4->text().isEmpty() || ui->l4_2->text().isEmpty() || ui->l4_3->text().isEmpty() ||
        ui->l4_4->text().isEmpty() || ui->l4_5->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Remplir tous les champs !");
        return;
    }

    // CORRECTED FIELD MAPPING:
    // Get the next available ID
    QSqlQuery idQuery;
    idQuery.prepare("SELECT NVL(MAX(ID_LOCAL), 0) + 1 AS NEXT_ID FROM LOCAUX");
    int id = 1;
    if (idQuery.exec() && idQuery.next()) {
        id = idQuery.value("NEXT_ID").toInt();
    }
    
    QString type = ui->l1->text();             // Type from l1 (labeled as "Type de local")
    QString ville = ui->l2->text();            // Ville from l2
    QString adresse = ui->l3->text();          // Adresse from l3
    int dispo = ui->l4->text().toInt();        // Disponibilité from l4
    QString nom = ui->l4_2->text();            // Nom propriétaire from l4_2
    QString cin = ui->l4_5->text();            // CIN from l4_5
    QString tel = ui->l4_3->text();            // Téléphone from l4_3
    double prix = ui->l4_4->text().toDouble(); // Prix from l4_4

    if (dispo != 0 && dispo != 1) {
        QMessageBox::warning(this, "Erreur", "Disponibilité: 0 ou 1 !");
        return;
    }

    Local local(id, type, ville, adresse, dispo, nom, cin, tel, prix);

    if (local.ajouter()) {
        QMessageBox::information(this, "Succès", "Local ajouté !");
        afficherTable();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec ajout !");
    }
}
void LocalWindow::on_modifierButton_clicked()
{
    if (ui->lineedit_id_modif->text().isEmpty() || ui->l7->text().isEmpty() || ui->l8->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Remplir tous les champs !");
        return;
    }

    int id = ui->lineedit_id_modif->text().toInt();
    double nouveauPrix = ui->l7->text().toDouble();
    int nouvelleDispo = ui->l8->text().toInt();

    if (nouvelleDispo != 0 && nouvelleDispo != 1) {
        QMessageBox::warning(this, "Erreur", "Disponibilité: 0 ou 1 !");
        return;
    }

    Local* localExist = Local::rechercherParId(id);
    if (!localExist) {
        QMessageBox::warning(this, "Erreur", "Local non trouvé !");
        return;
    }

    Local local(id, localExist->getTypeLocal(), localExist->getVille(), localExist->getAdresse(),
                nouvelleDispo, localExist->getNomProprietaire(), localExist->getCinProprietaire(),
                localExist->getNumeroTel(), nouveauPrix);

    delete localExist;

    if (local.modifier()) {
        QMessageBox::information(this, "Succès", "Local modifié !");
        afficherTable();
        ui->lineedit_id_modif->clear();
        ui->l7->clear();
        ui->l8->clear();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec modification !");
    }
}

void LocalWindow::clearInputs()
{
    ui->l1->clear();
    ui->l2->clear();
    ui->l3->clear();
    ui->l4->clear();
    ui->l4_2->clear();
    ui->l4_3->clear();
    ui->l4_4->clear();
    ui->l4_5->clear();
}
