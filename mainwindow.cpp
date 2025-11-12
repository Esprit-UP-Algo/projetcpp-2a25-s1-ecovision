#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resource.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Manually connect buttons
    connect(ui->btn_ajouter, &QPushButton::clicked, this, &MainWindow::on_btn_ajouter_clicked);
    connect(ui->btn_supprimer, &QPushButton::clicked, this, &MainWindow::on_btn_supprimer_clicked);
    connect(ui->btn_modifier, &QPushButton::clicked, this, &MainWindow::on_btn_modifier_clicked);
    connect(ui->btn_afficher, &QPushButton::clicked, this, &MainWindow::on_btn_afficher_clicked);
    connect(ui->btn_recherche, &QPushButton::clicked, this, &MainWindow::on_btn_recherche_clicked);

    // Initialize database
    createResourceTable();
    afficherTable(); // show table at startup
}

MainWindow::~MainWindow()
{
    delete ui;
}

// =============================
// Create table if not exists
// =============================
bool MainWindow::createResourceTable()
{
    QSqlQuery query;
    QString createTableSQL =
        "CREATE TABLE RESSOURCES ("
        "   ID NUMBER PRIMARY KEY, "
        "   TYPE VARCHAR2(100), "
        "   LOCALISATION VARCHAR2(100), "
        "   CONSOMMATION NUMBER, "
        "   DATE_MESURE DATE, "
        "   PRIX NUMBER"
        ")";

    if (!query.exec(createTableSQL)) {
        QSqlError error = query.lastError();
        // If table already exists (ORA-00955), that's fine
        if (!error.databaseText().contains("ORA-00955")) {
            qDebug() << "Table creation warning:" << error.text();
        } else {
            qDebug() << "Table RESSOURCES already exists";
        }
    } else {
        qDebug() << "Table RESSOURCES created successfully";
    }

    return true;
}

// =============================
// Check if table exists
// =============================
bool MainWindow::checkTableExists()
{
    QSqlQuery query;
    return query.exec("SELECT 1 FROM RESSOURCES WHERE 1=0");
}

// =============================
// Helper: get type from radio buttons
// =============================
QString MainWindow::getSelectedType() const
{
    if (ui->radio_eau->isChecked())
        return "Eau";
    else if (ui->radio_elec->isChecked())
        return "Electricite";
    else
        return "";
}

// =============================
// Display / Refresh all resources
// =============================
void MainWindow::afficherTable()
{
    QSqlQuery query;

    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX FROM RESSOURCES ORDER BY ID");

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des données: " + error.text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(
        {"ID", "Type", "Localisation", "Consommation", "Date Mesure", "Prix"}
        );

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value("ID").toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value("TYPE").toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value("LOCALISATION").toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value("CONSOMMATION").toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value("DATE_MESURE").toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value("PRIX").toString()));
        row++;
    }

    ui->tableWidget->resizeColumnsToContents();
}

// =============================
// Ajouter
// =============================
void MainWindow::on_btn_ajouter_clicked()
{
    // Validate input
    if (ui->lineEdit_id->text().isEmpty() ||
        ui->lineEdit_localisation->text().isEmpty() ||
        ui->lineEdit_consommation->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    QString type = getSelectedType();
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un type !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QString localisation = ui->lineEdit_localisation->text();
    double consommation = ui->lineEdit_consommation->text().toDouble();
    QString date_mesure = ui->dateEdit_mesure->date().toString("dd/MM/yyyy");

    double prix = 0;
    if (type == "Eau")
        prix = consommation * 0.5;
    else if (type == "Electricite")
        prix = consommation * 0.8;

    Resource r(id, type, localisation, consommation, date_mesure, prix);

    if (r.ajouter()) {
        QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès !");
        afficherTable();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de la ressource !");
    }
}

// =============================
// Supprimer
// =============================
void MainWindow::on_btn_supprimer_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID à supprimer !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();

    // Ask for confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer la ressource ID " + QString::number(id) + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Resource r;
        if (r.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Ressource supprimée !");
            afficherTable();
            clearInputs();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression ! ID non trouvé.");
        }
    }
}

// =============================
// Modifier
// =============================
void MainWindow::on_btn_modifier_clicked()
{
    // Validate input
    if (ui->lineEdit_id->text().isEmpty() ||
        ui->lineEdit_localisation->text().isEmpty() ||
        ui->lineEdit_consommation->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    QString type = getSelectedType();
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un type !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QString localisation = ui->lineEdit_localisation->text();
    double consommation = ui->lineEdit_consommation->text().toDouble();
    QString date_mesure = ui->dateEdit_mesure->date().toString("dd/MM/yyyy");

    double prix = 0;
    if (type == "Eau")
        prix = consommation * 0.5;
    else if (type == "Electricite")
        prix = consommation * 0.8;

    Resource r(id, type, localisation, consommation, date_mesure, prix);

    if (r.modifier()) {
        QMessageBox::information(this, "Succès", "Ressource modifiée !");
        afficherTable();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification ! ID non trouvé.");
    }
}

// =============================
// Afficher button (refresh)
// =============================
void MainWindow::on_btn_afficher_clicked()
{
    afficherTable();
}

// =============================
// Recherche button
// =============================
void MainWindow::on_btn_recherche_clicked()
{
    // Add your search functionality here
    QMessageBox::information(this, "Recherche", "Fonctionnalité de recherche à implémenter");
}

// =============================
// Clear input fields
// =============================
void MainWindow::clearInputs()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_localisation->clear();
    ui->lineEdit_consommation->clear();
    ui->dateEdit_mesure->setDate(QDate::currentDate());
    // Clear radio buttons
    ui->radio_eau->setAutoExclusive(false);
    ui->radio_elec->setAutoExclusive(false);
    ui->radio_eau->setChecked(false);
    ui->radio_elec->setChecked(false);
    ui->radio_eau->setAutoExclusive(true);
    ui->radio_elec->setAutoExclusive(true);
}

// =============================
// When table row is clicked, load data into form
// =============================
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= ui->tableWidget->rowCount()) return;

    if (ui->tableWidget->item(row, 0)) {
        ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());
    }

    if (ui->tableWidget->item(row, 1)) {
        QString type = ui->tableWidget->item(row, 1)->text();
        if (type == "Eau") {
            ui->radio_eau->setChecked(true);
        } else if (type == "Electricite") {
            ui->radio_elec->setChecked(true);
        }
    }

    if (ui->tableWidget->item(row, 2)) {
        ui->lineEdit_localisation->setText(ui->tableWidget->item(row, 2)->text());
    }

    if (ui->tableWidget->item(row, 3)) {
        ui->lineEdit_consommation->setText(ui->tableWidget->item(row, 3)->text());
    }

    if (ui->tableWidget->item(row, 4)) {
        // Parse date from DD/MM/YYYY format
        QString dateStr = ui->tableWidget->item(row, 4)->text();
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (date.isValid()) {
            ui->dateEdit_mesure->setDate(date);
        }
    }
}
