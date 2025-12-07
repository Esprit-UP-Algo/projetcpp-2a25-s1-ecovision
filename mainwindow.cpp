#include "mainwindow.h"
#include "qboxlayout.h"
#include "qgroupbox.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "resource.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>
#include <QDate>
#include <QTextDocument>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QValueAxis>
#include "libs/qrcodegen/QrCode.hpp"
#include <stdexcept>
#include <QImage>
#include <QDateTime>
#include <QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QImageWriter>
#include "StatWindow.h"
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setWindowTitle("EcoVision Pro - Gestion Intégrée");
    createResourceTable();
    // Initialize Employee Table
    ui->tableWidget_3->setColumnCount(8);
    ui->tableWidget_3->setHorizontalHeaderLabels(
        QStringList() << "CIN" << "Nom" << "Prénom" << "Téléphone"
                      << "Adresse" << "Métier" << "Présence" << "Salaire");
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_3->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_3->setAlternatingRowColors(true);

    // Initialize Resource Tables
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(
        {"ID", "Type", "Localisation", "Consommation", "Date Mesure", "Prix"});
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setAlternatingRowColors(true);

    ui->tableWidget_2->setColumnCount(7);
    ui->tableWidget_2->setHorizontalHeaderLabels(
        {"Type", "Nombre", "Consommation Total", "Prix Total", "Prix Moyen", "Prix Min", "Prix Max"});

    // Connect Employee Signals
    connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::on_btnAjouter_clicked);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::on_btnModifier_clicked);
    connect(ui->btnSupprimer, &QPushButton::clicked, this, &MainWindow::on_btnSupprimer_clicked);
    connect(ui->btntrier, &QPushButton::clicked, this, &MainWindow::on_btntrier_clicked);
    connect(ui->btnpdf, &QPushButton::clicked, this, &MainWindow::on_btnpdf_clicked);
    connect(ui->cin_4, &QLineEdit::textChanged, this, &MainWindow::on_cin_4_textChanged);
    connect(ui->tableWidget_3, &QTableWidget::cellClicked, this, &MainWindow::on_tableWidget_3_cellClicked);
    // In MainWindow constructor, add:
    connect(ui->btn_qrcode, &QPushButton::clicked, this, &MainWindow::on_btn_qrcode_clicked);
    // Add this connection in constructor:

    // Connect Resource Signals
    connect(ui->btn_ajouter, &QPushButton::clicked, this, &MainWindow::on_btn_ajouter_clicked);
    connect(ui->btn_modifier, &QPushButton::clicked, this, &MainWindow::on_btn_modifier_clicked);
    connect(ui->btn_supprimer, &QPushButton::clicked, this, &MainWindow::on_btn_supprimer_clicked);
    connect(ui->btn_afficher_id, &QPushButton::clicked, this, &MainWindow::on_btn_afficher_id_clicked);
    connect(ui->btn_afficher_prix, &QPushButton::clicked, this, &MainWindow::on_btn_afficher_prix_clicked);
    connect(ui->btn_recherche, &QPushButton::clicked, this, &MainWindow::on_btn_recherche_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
    connect(ui->btn_pdf_avance, &QPushButton::clicked, this, &MainWindow::on_btn_pdf_avance_clicked);
    connect(ui->statsButton, &QPushButton::clicked, this, &MainWindow::on_statsButton_clicked);
    connect(ui->btn_afficher, &QPushButton::clicked, this, &MainWindow::on_btn_afficher_clicked);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::on_tableWidget_cellClicked);

    // Connect tab change
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tabWidget_currentChanged);

    // Load initial data
    loadEmployeeData();
    loadResourceData();
    loadResourceStats();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== EMPLOYEE METHODS ====================

void MainWindow::loadEmployeeData()
{
    ui->tableWidget_3->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT cin, nom, prenom, telephone, adresse, metier, presence, salaire FROM EMPLOYEE ORDER BY cin");

    if (!query.exec()) {
        qDebug() << "Employee load error:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_3->insertRow(row);
        for (int col = 0; col < 8; col++) {
            ui->tableWidget_3->setItem(row, col,
                                       new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    ui->tableWidget_3->resizeColumnsToContents();
}

void MainWindow::clearEmployeeFields()
{
    ui->cin_2->clear();
    ui->nom_2->clear();
    ui->prenom_2->clear();
    ui->telephone_2->clear();
    ui->adresse_2->clear();
    ui->metier_2->clear();
    ui->presence_2->clear();
    ui->salaire_2->clear();
}

bool MainWindow::validateEmployeeInput()
{
    QString cin = ui->cin_2->text().trimmed();
    QString nom = ui->nom_2->text().trimmed();
    QString prenom = ui->prenom_2->text().trimmed();
    QString salaire = ui->salaire_2->text().trimmed();

    // Check CIN
    if (cin.length() != 8 || !cin.toInt()) {
        QMessageBox::warning(this, "Erreur", "CIN doit être 8 chiffres!");
        return false;
    }

    // Check required fields
    if (nom.isEmpty() || prenom.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Nom et Prénom sont requis!");
        return false;
    }

    // Check salary
    if (!salaire.isEmpty() && !salaire.toDouble()) {
        QMessageBox::warning(this, "Erreur", "Salaire invalide!");
        return false;
    }

    return true;
}

void MainWindow::on_btnAjouter_clicked()
{
    if (!validateEmployeeInput()) return;

    QString cin = ui->cin_2->text().trimmed();
    QString nom = ui->nom_2->text().trimmed();
    QString prenom = ui->prenom_2->text().trimmed();
    QString telephone = ui->telephone_2->text().trimmed();
    QString adresse = ui->adresse_2->text().trimmed();
    QString metier = ui->metier_2->text().trimmed();
    QString presence = ui->presence_2->text().trimmed();
    double salaire = ui->salaire_2->text().toDouble();

    Employee emp(cin, nom, prenom, adresse, telephone, metier, salaire, presence);

    if (emp.addToDatabase()) {
        QMessageBox::information(this, "Succès", "Employé ajouté!");
        clearEmployeeFields();
        loadEmployeeData();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout!");
    }
}

void MainWindow::on_btnModifier_clicked()
{
    if (!validateEmployeeInput()) return;

    QString cin = ui->cin_2->text().trimmed();
    QString nom = ui->nom_2->text().trimmed();
    QString prenom = ui->prenom_2->text().trimmed();
    QString telephone = ui->telephone_2->text().trimmed();
    QString adresse = ui->adresse_2->text().trimmed();
    QString metier = ui->metier_2->text().trimmed();
    QString presence = ui->presence_2->text().trimmed();
    double salaire = ui->salaire_2->text().toDouble();

    Employee emp(cin, nom, prenom, adresse, telephone, metier, salaire, presence);

    if (emp.updateInDatabase()) {
        QMessageBox::information(this, "Succès", "Employé modifié!");
        clearEmployeeFields();
        loadEmployeeData();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification!");
    }
}

void MainWindow::on_btnSupprimer_clicked()
{
    QString cin = ui->cin_2->text().trimmed();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Supprimer l'employé CIN: " + cin + "?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Employee::deleteByCin(cin)) {
            QMessageBox::information(this, "Succès", "Employé supprimé!");
            clearEmployeeFields();
            loadEmployeeData();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de suppression!");
        }
    }
}

void MainWindow::on_btntrier_clicked()
{
    QVector<Employee> employees = Employee::fetchAllSortedBySalaire();
    ui->tableWidget_3->setRowCount(0);

    for (const Employee &emp : employees) {
        int row = ui->tableWidget_3->rowCount();
        ui->tableWidget_3->insertRow(row);
        ui->tableWidget_3->setItem(row, 0, new QTableWidgetItem(emp.getCin()));
        ui->tableWidget_3->setItem(row, 1, new QTableWidgetItem(emp.getNom()));
        ui->tableWidget_3->setItem(row, 2, new QTableWidgetItem(emp.getPrenom()));
        ui->tableWidget_3->setItem(row, 3, new QTableWidgetItem(emp.getTelephone()));
        ui->tableWidget_3->setItem(row, 4, new QTableWidgetItem(emp.getAdresse()));
        ui->tableWidget_3->setItem(row, 5, new QTableWidgetItem(emp.getMetier()));
        ui->tableWidget_3->setItem(row, 6, new QTableWidgetItem(emp.getPresence()));
        ui->tableWidget_3->setItem(row, 7,
                                   new QTableWidgetItem(QString::number(emp.getSalaire(), 'f', 2)));
    }
}

void MainWindow::on_tableWidget_3_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= ui->tableWidget_3->rowCount()) return;

    ui->cin_2->setText(ui->tableWidget_3->item(row, 0)->text());
    ui->nom_2->setText(ui->tableWidget_3->item(row, 1)->text());
    ui->prenom_2->setText(ui->tableWidget_3->item(row, 2)->text());
    ui->telephone_2->setText(ui->tableWidget_3->item(row, 3)->text());
    ui->adresse_2->setText(ui->tableWidget_3->item(row, 4)->text());
    ui->metier_2->setText(ui->tableWidget_3->item(row, 5)->text());
    ui->presence_2->setText(ui->tableWidget_3->item(row, 6)->text());
    ui->salaire_2->setText(ui->tableWidget_3->item(row, 7)->text());
}

void MainWindow::on_cin_4_textChanged(const QString &text)
{
    QString search = text.trimmed();
    if (search.isEmpty()) {
        loadEmployeeData();
        return;
    }

    ui->tableWidget_3->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT cin, nom, prenom, telephone, adresse, metier, presence, salaire "
                  "FROM EMPLOYEE WHERE cin LIKE ? OR nom LIKE ? OR prenom LIKE ?");
    query.bindValue(0, "%" + search + "%");
    query.bindValue(1, "%" + search + "%");
    query.bindValue(2, "%" + search + "%");

    if (query.exec()) {
        int row = 0;
        while (query.next()) {
            ui->tableWidget_3->insertRow(row);
            for (int col = 0; col < 8; col++) {
                ui->tableWidget_3->setItem(row, col,
                                           new QTableWidgetItem(query.value(col).toString()));
            }
            row++;
        }
        ui->tableWidget_3->resizeColumnsToContents();
    }
}


// ==================== RESOURCE METHODS ====================

void MainWindow::loadResourceData()
{
    ui->tableWidget->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY'), PRIX FROM RESSOURCES ORDER BY ID");

    if (!query.exec()) {
        qDebug() << "Resource load error:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 6; col++) {
            ui->tableWidget->setItem(row, col,
                                     new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::loadResourceStats()
{
    ui->tableWidget_2->setRowCount(0);

    QSqlQuery query;
    query.prepare("SELECT "
                  "TYPE, "
                  "COUNT(*) as nombre, "
                  "SUM(CONSOMMATION) as total_consommation, "
                  "SUM(PRIX) as total_prix, "
                  "AVG(PRIX) as prix_moyen, "
                  "MIN(PRIX) as prix_min, "
                  "MAX(PRIX) as prix_max "
                  "FROM RESSOURCES "
                  "GROUP BY TYPE "
                  "ORDER BY TYPE");

    if (!query.exec()) {
        qDebug() << "Stats error:" << query.lastError().text();
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);

        // Get values
        QString type = query.value("TYPE").toString();
        int count = query.value("nombre").toInt();
        double totalCons = query.value("total_consommation").toDouble();
        double totalPrix = query.value("total_prix").toDouble();
        double avgPrix = query.value("prix_moyen").toDouble();
        double minPrix = query.value("prix_min").toDouble();
        double maxPrix = query.value("prix_max").toDouble();

        // Format type with icon
        QString typeDisplay = type;
        if (type == "Eau") typeDisplay = "💧 " + type;
        else if (type == "Electricite") typeDisplay = "⚡ " + type;

        // Set items
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(typeDisplay));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(QString::number(count)));
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(QString::number(totalCons, 'f', 2) + " kWh"));
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(QString::number(totalPrix, 'f', 2) + " DT"));
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(QString::number(avgPrix, 'f', 2) + " DT"));
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(QString::number(minPrix, 'f', 2) + " DT"));
        ui->tableWidget_2->setItem(row, 6, new QTableWidgetItem(QString::number(maxPrix, 'f', 2) + " DT"));

        // Color code rows
        for (int col = 0; col < 7; col++) {
            QTableWidgetItem *item = ui->tableWidget_2->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);

                // Color based on type
                if (type == "Eau") {
                    item->setBackground(QColor(173, 216, 230)); // Light blue
                } else if (type == "Electricite") {
                    item->setBackground(QColor(255, 182, 193)); // Light pink
                }
            }
        }

        row++;
    }

    // Add total row
    QSqlQuery totalQuery;
    totalQuery.prepare("SELECT "
                       "COUNT(*) as total_nombre, "
                       "SUM(CONSOMMATION) as total_cons_all, "
                       "SUM(PRIX) as total_prix_all "
                       "FROM RESSOURCES");

    if (totalQuery.exec() && totalQuery.next()) {
        ui->tableWidget_2->insertRow(row);

        int totalCount = totalQuery.value("total_nombre").toInt();
        double totalConsAll = totalQuery.value("total_cons_all").toDouble();
        double totalPrixAll = totalQuery.value("total_prix_all").toDouble();

        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem("📊 TOTAL"));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(QString::number(totalCount)));
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(QString::number(totalConsAll, 'f', 2) + " kWh"));
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(QString::number(totalPrixAll, 'f', 2) + " DT"));

        // Color the total row
        for (int col = 0; col < 7; col++) {
            QTableWidgetItem *item = ui->tableWidget_2->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
                item->setBackground(QColor(144, 238, 144)); // Light green
                item->setFont(QFont("Arial", 10, QFont::Bold));
            }
        }
    }

    ui->tableWidget_2->resizeColumnsToContents();
}
void MainWindow::clearResourceFields()
{
    ui->lineEdit_id->clear();
    ui->lineEdit_localisation->clear();
    ui->lineEdit_consommation->clear();
    ui->dateEdit_mesure->setDate(QDate::currentDate());
    ui->radio_eau->setAutoExclusive(false);
    ui->radio_elec->setAutoExclusive(false);
    ui->radio_eau->setChecked(false);
    ui->radio_elec->setChecked(false);
    ui->radio_eau->setAutoExclusive(true);
    ui->radio_elec->setAutoExclusive(true);
}

QString MainWindow::getResourceType() const
{
    if (ui->radio_eau->isChecked()) return "Eau";
    if (ui->radio_elec->isChecked()) return "Electricite";
    return "";
}

bool MainWindow::validateResourceInput()
{
    QString idText = ui->lineEdit_id->text().trimmed();
    QString localisation = ui->lineEdit_localisation->text().trimmed();
    QString consommation = ui->lineEdit_consommation->text().trimmed();
    QString type = getResourceType();

    // Check ID
    if (idText.isEmpty() || !idText.toInt()) {
        QMessageBox::warning(this, "Erreur", "ID invalide!");
        return false;
    }

    // Check type
    if (type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez un type!");
        return false;
    }

    // Check required fields
    if (localisation.isEmpty() || consommation.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont requis!");
        return false;
    }

    // Check consommation
    if (!consommation.toDouble()) {
        QMessageBox::warning(this, "Erreur", "Consommation invalide!");
        return false;
    }

    return true;
}

void MainWindow::on_btn_ajouter_clicked()
{
    if (!validateResourceInput()) return;

    int id = ui->lineEdit_id->text().toInt();
    QString type = getResourceType();
    QString localisation = ui->lineEdit_localisation->text().trimmed();
    double consommation = ui->lineEdit_consommation->text().toDouble();
    QString date_mesure = ui->dateEdit_mesure->date().toString("dd/MM/yyyy");
    double prix = (type == "Eau") ? consommation * 0.5 : consommation * 0.8;

    Resource res(id, type, localisation, consommation, date_mesure, prix);

    if (res.ajouter()) {
        QMessageBox::information(this, "Succès", "Ressource ajoutée!");
        clearResourceFields();
        loadResourceData();
        loadResourceStats();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout!");
    }
}

void MainWindow::on_btn_modifier_clicked()
{
    if (!validateResourceInput()) return;

    int id = ui->lineEdit_id->text().toInt();
    QString type = getResourceType();
    QString localisation = ui->lineEdit_localisation->text().trimmed();
    double consommation = ui->lineEdit_consommation->text().toDouble();
    QString date_mesure = ui->dateEdit_mesure->date().toString("dd/MM/yyyy");
    double prix = (type == "Eau") ? consommation * 0.5 : consommation * 0.8;

    Resource res(id, type, localisation, consommation, date_mesure, prix);

    if (res.modifier()) {
        QMessageBox::information(this, "Succès", "Ressource modifiée!");
        clearResourceFields();
        loadResourceData();
        loadResourceStats();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification!");
    }
}

void MainWindow::on_btn_supprimer_clicked()
{
    QString idText = ui->lineEdit_id->text().trimmed();

    if (idText.isEmpty() || !idText.toInt()) {
        QMessageBox::warning(this, "Erreur", "ID invalide!");
        return;
    }

    int id = idText.toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Supprimer la ressource ID: " + QString::number(id) + "?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Resource res;
        if (res.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Ressource supprimée!");
            clearResourceFields();
            loadResourceData();
            loadResourceStats();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de suppression!");
        }
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= ui->tableWidget->rowCount()) return;

    ui->lineEdit_id->setText(ui->tableWidget->item(row, 0)->text());

    QString type = ui->tableWidget->item(row, 1)->text();
    ui->radio_eau->setChecked(type == "Eau");
    ui->radio_elec->setChecked(type == "Electricite");

    ui->lineEdit_localisation->setText(ui->tableWidget->item(row, 2)->text());
    ui->lineEdit_consommation->setText(ui->tableWidget->item(row, 3)->text());

    QString dateStr = ui->tableWidget->item(row, 4)->text();
    QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
    if (date.isValid()) {
        ui->dateEdit_mesure->setDate(date);
    }
}





void MainWindow::on_btn_afficher_clicked()
{
      loadResourceData();

}

void MainWindow::on_pushButton_4_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    QString html = "<html><body>";
    html += "<h1>Liste des Ressources</h1>";
    html += "<p>Exporté le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";
    html += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
    html += "<tr><th>ID</th><th>Type</th><th>Localisation</th><th>Consommation</th><th>Date</th><th>Prix</th></tr>";

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < 6; ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            html += "<td>" + (item ? item->text() : "") + "</td>";
        }
        html += "</tr>";
    }

    html += "</table></body></html>";
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "PDF exporté!");
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) { // Resource tab
        loadResourceData();
    } else if (index == 1) { // Employee tab
        loadEmployeeData();
    }
}
// ==================== MISSING RESOURCE METHODS ====================

bool MainWindow::createResourceTable()
{
    QSqlQuery query;

    // Check if table already exists
    if (!query.exec("SELECT COUNT(*) FROM RESSOURCES")) {
        QString createTableSQL =
            "CREATE TABLE RESSOURCES ("
            "   ID INTEGER PRIMARY KEY, "
            "   TYPE VARCHAR(100), "
            "   LOCALISATION VARCHAR(100), "
            "   CONSOMMATION REAL, "
            "   DATE_MESURE DATE, "
            "   PRIX REAL, "
            "   RESIDENT_ID INTEGER"
            ")";

        if (!query.exec(createTableSQL)) {
            qDebug() << "Table creation error:" << query.lastError().text();
            return false;
        }
    }
    return true;
}
void MainWindow::chargerDonneesParID(int id)
{
    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                  "FROM RESSOURCES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche: " + error.text());
        return;
    }

    if (query.next()) {
        ui->lineEdit_id->setText(query.value("ID").toString());

        QString type = query.value("TYPE").toString();
        if (type == "Eau") {
            ui->radio_eau->setChecked(true);
        } else if (type == "Electricite") {
            ui->radio_elec->setChecked(true);
        }

        ui->lineEdit_localisation->setText(query.value("LOCALISATION").toString());
        ui->lineEdit_consommation->setText(query.value("CONSOMMATION").toString());

        QString dateStr = query.value("DATE_MESURE").toString();
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (date.isValid()) {
            ui->dateEdit_mesure->setDate(date);
        }

        QMessageBox::information(this, "Succès", "Données chargées avec succès ! Vous pouvez maintenant les modifier.");
    } else {
        QMessageBox::information(this, "Recherche", "Aucune ressource trouvée avec cet ID.");
    }
}

void MainWindow::afficherStatistiques()
{
    QSqlQuery query;
    query.prepare("SELECT "
                  "TYPE, "
                  "COUNT(*) as nombre, "
                  "SUM(CONSOMMATION) as total_consommation, "
                  "SUM(PRIX) as total_prix, "
                  "AVG(PRIX) as prix_moyen, "
                  "MIN(PRIX) as prix_min, "
                  "MAX(PRIX) as prix_max "
                  "FROM RESSOURCES "
                  "GROUP BY TYPE "
                  "ORDER BY TYPE");

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des statistiques: " + error.text());
        return;
    }

    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(7);
    ui->tableWidget_2->setHorizontalHeaderLabels(
        {"Type", "Nombre", "Consommation Total", "Prix Total", "Prix Moyen", "Prix Min", "Prix Max"}
        );

    int row = 0;
    while (query.next()) {
        ui->tableWidget_2->insertRow(row);
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(query.value("TYPE").toString()));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(query.value("nombre").toString()));
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("total_consommation").toDouble(), 'f', 2)));
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(QString::number(query.value("total_prix").toDouble(), 'f', 2)));
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(QString::number(query.value("prix_moyen").toDouble(), 'f', 2)));
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(QString::number(query.value("prix_min").toDouble(), 'f', 2)));
        ui->tableWidget_2->setItem(row, 6, new QTableWidgetItem(QString::number(query.value("prix_max").toDouble(), 'f', 2)));
        row++;
    }

    ui->tableWidget_2->resizeColumnsToContents();
}

int MainWindow::getDefaultResidentId()
{
    QSqlQuery query;
    query.prepare("SELECT ID FROM RESIDENT WHERE ROWNUM = 1 ORDER BY ID");

    if (query.exec() && query.next()) {
        return query.value("ID").toInt();
    }

    // Si aucun résident n'existe, retourner -1
    return -1;
}

void MainWindow::genererPDFAvance()
{
    // Get the ID entered by the user
    QString idText = ui->lineEdit_id->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID de résident !");
        return;
    }

    bool ok;
    int residentIdInput = idText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    genererPDFGaranti(residentIdInput);
}



void MainWindow::genererPDFGaranti(int residentIdInput)
{
    qDebug() << "=== DEBUG START ===";
    qDebug() << "Input resident ID:" << residentIdInput;

    // Test database connection
    QSqlDatabase db = QSqlDatabase::database();
    qDebug() << "Database connected:" << db.isOpen();

    // 1. Check if resident exists
    QSqlQuery residentQuery;
    residentQuery.prepare("SELECT FIRST_NAME, LAST_NAME FROM RESIDENT WHERE ID = ?");
    residentQuery.bindValue(0, residentIdInput);

    if (!residentQuery.exec()) {
        qDebug() << "Resident query error:" << residentQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur base de données: " + residentQuery.lastError().text());
        return;
    }

    if (!residentQuery.next()) {
        qDebug() << "Resident not found for ID:" << residentIdInput;
        QMessageBox::warning(this, "Erreur", QString("Résident ID %1 non trouvé!").arg(residentIdInput));
        return;
    }

    QString firstName = residentQuery.value("FIRST_NAME").toString();
    QString lastName = residentQuery.value("LAST_NAME").toString();
    QString fullName = firstName + " " + lastName;

    // 2. Check resources for THIS resident
    QSqlQuery resourcesQuery;
    resourcesQuery.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                           "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                           "FROM RESSOURCES WHERE RESIDENT_ID = ? ORDER BY ID");
    resourcesQuery.bindValue(0, residentIdInput);

    if (!resourcesQuery.exec()) {
        qDebug() << "Resources query error:" << resourcesQuery.lastError().text();
        QMessageBox::critical(this, "Erreur", "Erreur requête: " + resourcesQuery.lastError().text());
        return;
    }

    // Create PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer PDF Avancé",
                                                    QString("rapport_resident_%1.pdf").arg(residentIdInput),
                                                    "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; margin: 20px; }"
                   "h1 { color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 10px; }"
                   "h2 { color: #34495e; margin-top: 20px; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; }"
                   "th { background-color: #3498db; color: white; padding: 12px; text-align: left; }"
                   "td { padding: 10px; border: 1px solid #ddd; }"
                   ".info-box { background-color: #ecf0f1; padding: 15px; margin: 15px 0; border-radius: 5px; }"
                   ".total-box { background-color: #2ecc71; color: white; padding: 15px; margin: 20px 0; border-radius: 5px; }"
                   "</style></head><body>";

    html += QString("<h1>Rapport Avancé - Résident</h1>");
    html += "<div class='info-box'>";
    html += QString("<p><strong>Nom:</strong> %1</p>").arg(fullName);
    html += QString("<p><strong>ID Résident:</strong> %1</p>").arg(residentIdInput);
    html += QString("<p><strong>Date de génération:</strong> %1</p>").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    html += "</div>";

    html += "<h2>Liste des Ressources</h2>";
    html += "<table>";
    html += "<tr><th>ID</th><th>Type</th><th>Localisation</th><th>Consommation</th><th>Date</th><th>Prix (DT)</th></tr>";

    int count = 0;
    double totalPrix = 0;
    while (resourcesQuery.next()) {
        count++;
        double prix = resourcesQuery.value("PRIX").toDouble();
        totalPrix += prix;

        html += "<tr>";
        html += "<td>" + resourcesQuery.value("ID").toString() + "</td>";
        html += "<td>" + resourcesQuery.value("TYPE").toString() + "</td>";
        html += "<td>" + resourcesQuery.value("LOCALISATION").toString() + "</td>";
        html += "<td>" + resourcesQuery.value("CONSOMMATION").toString() + "</td>";
        html += "<td>" + resourcesQuery.value("DATE_MESURE").toString() + "</td>";
        html += "<td>" + QString::number(prix, 'f', 2) + "</td>";
        html += "</tr>";
    }

    html += "</table>";

    if (count > 0) {
        html += "<div class='total-box'>";
        html += QString("<p><strong>Nombre de ressources:</strong> %1</p>").arg(count);
        html += QString("<p><strong>Coût total:</strong> %1 DT</p>").arg(QString::number(totalPrix, 'f', 2));
        html += QString("<p><strong>Coût moyen par ressource:</strong> %1 DT</p>").arg(QString::number(totalPrix/count, 'f', 2));
        html += "</div>";
    } else {
        html += "<div class='info-box'>";
        html += "<p style='color: #e74c3c;'><strong>Aucune ressource trouvée pour ce résident.</strong></p>";
        html += "</div>";
    }

    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Succès",
                             QString("PDF avancé généré avec succès !\n"
                                     "Résident: %1\n"
                                     "Ressources: %2\n"
                                     "Coût total: %3 DT\n"
                                     "Fichier: %4")
                                 .arg(fullName)
                                 .arg(count)
                                 .arg(QString::number(totalPrix, 'f', 2))
                                 .arg(fileName));

    qDebug() << "=== DEBUG END ===";
}// Update the on_btn_pdf_avance_clicked method to use genererPDFAvance:

// Update the on_btn_qrcode_clicked method:


// Update the on_statsButton_clicked method:
void MainWindow::on_statsButton_clicked()
{
    // Create and show the statistics window
    StatWindow *statWindow = new StatWindow(this);
    statWindow->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed
    statWindow->exec(); // Show as modal dialog
}
// Update the on_btn_recherche_clicked method:
void MainWindow::on_btn_recherche_clicked()
{
    QString idText = ui->lineEdit_id->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID !");
        return;
    }

    int id = idText.toInt();
    chargerDonneesParID(id);
}

// Update the constructor to call createResourceTable:
// ================== COMPLETE RESOURCE METHODS ==================

void MainWindow::on_btn_afficher_id_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();

    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                  "FROM RESSOURCES WHERE ID = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la recherche: " + query.lastError().text());
        return;
    }

    // Clear the tableWidget (not tableWidgetResource)
    ui->tableWidget->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 6; col++) {
            ui->tableWidget->setItem(row, col,
                                     new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }

    if (row == 0) {
        QMessageBox::information(this, "Information", "Aucune ressource trouvée avec cet ID.");
    }

    ui->tableWidget->resizeColumnsToContents();
}
void MainWindow::on_btn_afficher_prix_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, PRIX FROM RESSOURCES ORDER BY ID");

    if (!query.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des prix.");
        return;
    }

    ui->listWidget->clear();

    double totalPrix = 0;
    int count = 0;

    while (query.next()) {
        int id = query.value("ID").toInt();
        QString type = query.value("TYPE").toString();
        double prix = query.value("PRIX").toDouble();

        QString itemText = QString("ID: %1 | Type: %2 | Prix: %3 DT")
                               .arg(id)
                               .arg(type)
                               .arg(prix, 0, 'f', 2);

        ui->listWidget->addItem(itemText);
        totalPrix += prix;
        count++;
    }

    if (count > 0) {
        ui->listWidget->addItem("");
        ui->listWidget->addItem(QString("=== RÉSUMÉ ==="));
        ui->listWidget->addItem(QString("Total ressources: %1").arg(count));
        ui->listWidget->addItem(QString("Prix total: %1 DT").arg(totalPrix, 0, 'f', 2));
        ui->listWidget->addItem(QString("Prix moyen: %1 DT").arg(totalPrix/count, 0, 'f', 2));
    }
}

void MainWindow::on_btnpdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF",
                                                    "rapport_ressources.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QTextDocument document;
    QString html = "<html><head><style>"
                   "body { font-family: Arial, sans-serif; margin: 20px; }"
                   "h1 { color: #2c3e50; text-align: center; font-size: 24px; }"
                   "h2 { color: #34495e; border-bottom: 1px solid #bdc3c7; margin-top: 20px; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 12px; }"
                   "th { background-color: #3498db; color: white; padding: 10px; text-align: left; }"
                   "td { padding: 8px; border: 1px solid #ddd; }"
                   ".summary { background-color: #ecf0f1; padding: 15px; margin: 10px 0; }"
                   "</style></head><body>";

    html += "<h1>Rapport des Ressources</h1>";
    html += "<p>Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";
    html += "<h2>Statistiques</h2>";

    QSqlQuery statsQuery;
    statsQuery.prepare("SELECT COUNT(*) as total, SUM(PRIX) as total_prix, AVG(PRIX) as moyenne_prix, "
                       "SUM(CASE WHEN TYPE = 'Eau' THEN 1 ELSE 0 END) as count_eau, "
                       "SUM(CASE WHEN TYPE = 'Electricite' THEN 1 ELSE 0 END) as count_elec FROM RESSOURCES");

    if (statsQuery.exec() && statsQuery.next()) {
        html += "<div class='summary'>";
        html += "<p><strong>Total des ressources:</strong> " + statsQuery.value("total").toString() + "</p>";
        html += "<p><strong>Prix total:</strong> " + QString::number(statsQuery.value("total_prix").toDouble(), 'f', 2) + " DT</p>";
        html += "<p><strong>Prix moyen:</strong> " + QString::number(statsQuery.value("moyenne_prix").toDouble(), 'f', 2) + " DT</p>";
        html += "<p><strong>Ressources Eau:</strong> " + statsQuery.value("count_eau").toString() + "</p>";
        html += "<p><strong>Ressources Électricité:</strong> " + statsQuery.value("count_elec").toString() + "</p>";
        html += "</div>";
    }

    html += "<h2>Liste des Ressources</h2>";
    html += "<table>";
    html += "<tr><th>ID</th><th>Type</th><th>Localisation</th><th>Consommation</th><th>Date</th><th>Prix (DT)</th></tr>";

    QSqlQuery dataQuery;
    dataQuery.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, TO_CHAR(DATE_MESURE, 'DD/MM/YYYY'), PRIX FROM RESSOURCES ORDER BY ID");

    if (dataQuery.exec()) {
        while (dataQuery.next()) {
            html += "<tr>";
            html += "<td>" + dataQuery.value(0).toString() + "</td>";
            html += "<td>" + dataQuery.value(1).toString() + "</td>";
            html += "<td>" + dataQuery.value(2).toString() + "</td>";
            html += "<td>" + dataQuery.value(3).toString() + "</td>";
            html += "<td>" + dataQuery.value(4).toString() + "</td>";
            html += "<td>" + QString::number(dataQuery.value(5).toDouble(), 'f', 2) + "</td>";
            html += "</tr>";
        }
    }

    html += "</table></body></html>";
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    document.print(&printer);
    QMessageBox::information(this, "Succès", "PDF généré avec succès: " + fileName);
}

void MainWindow::on_btn_pdf_avance_clicked()
{
    // This will generate a PDF for ALL resources (not by specific ID)

    QString fileName = QFileDialog::getSaveFileName(this, "PDF Avancé",
                                                    "rapport_toutes_ressources.pdf",
                                                    "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QTextDocument document;
    QString html = "<html><head><style>"
                   "body { font-family: 'Arial', sans-serif; margin: 25px; }"
                   "h1 { color: #2c3e50; text-align: center; padding-bottom: 10px; border-bottom: 2px solid #3498db; }"
                   "h2 { color: #34495e; margin-top: 30px; padding-left: 10px; border-left: 4px solid #3498db; }"
                   ".header-info { background-color: #f8f9fa; padding: 15px; border-radius: 5px; margin: 20px 0; }"
                   ".summary-box { background-color: #e8f4f8; padding: 20px; border-radius: 8px; margin: 25px 0; border-left: 5px solid #3498db; }"
                   "table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 12px; }"
                   "th { background-color: #3498db; color: white; padding: 12px; text-align: left; font-weight: bold; }"
                   "td { padding: 10px; border: 1px solid #ddd; }"
                   "tr:nth-child(even) { background-color: #f9f9f9; }"
                   "tr:hover { background-color: #f5f5f5; }"
                   ".total-row { background-color: #2ecc71 !important; color: white; font-weight: bold; }"
                   ".section-title { background-color: #ecf0f1; padding: 8px 15px; margin: 5px 0; border-radius: 4px; font-weight: bold; }"
                   "</style></head><body>";

    // Header
    html += "<h1>📊 Rapport Avancé - Toutes les Ressources</h1>";

    QDateTime currentTime = QDateTime::currentDateTime();
    html += "<div class='header-info'>";
    html += "<p><strong>📅 Date de génération :</strong> " + currentTime.toString("dd/MM/yyyy HH:mm") + "</p>";
    html += "<p><strong>📋 Nombre total de ressources :</strong> " + QString::number(ui->tableWidget->rowCount()) + "</p>";
    html += "</div>";

    // Summary Statistics
    html += "<h2>📈 Statistiques Générales</h2>";

    QSqlQuery statsQuery;
    statsQuery.prepare("SELECT "
                       "COUNT(*) as total, "
                       "SUM(CONSOMMATION) as total_consommation, "
                       "SUM(PRIX) as total_prix, "
                       "AVG(PRIX) as prix_moyen, "
                       "MIN(PRIX) as prix_min, "
                       "MAX(PRIX) as prix_max, "
                       "SUM(CASE WHEN TYPE = 'Eau' THEN 1 ELSE 0 END) as count_eau, "
                       "SUM(CASE WHEN TYPE = 'Electricite' THEN 1 ELSE 0 END) as count_elec "
                       "FROM RESSOURCES");

    if (statsQuery.exec() && statsQuery.next()) {
        html += "<div class='summary-box'>";

        // Total Count
        html += "<p><strong>🔢 Nombre total de ressources :</strong> " + statsQuery.value("total").toString() + "</p>";

        // By Type
        int countEau = statsQuery.value("count_eau").toInt();
        int countElec = statsQuery.value("count_elec").toInt();
        html += QString("<p><strong>💧 Ressources Eau :</strong> %1 (%2%)</p>")
                    .arg(countEau)
                    .arg(countEau > 0 ? QString::number((countEau * 100.0) / statsQuery.value("total").toInt(), 'f', 1) : "0");
        html += QString("<p><strong>⚡ Ressources Électricité :</strong> %1 (%2%)</p>")
                    .arg(countElec)
                    .arg(countElec > 0 ? QString::number((countElec * 100.0) / statsQuery.value("total").toInt(), 'f', 1) : "0");

        // Consumption
        html += "<p><strong>📊 Consommation totale :</strong> " +
                QString::number(statsQuery.value("total_consommation").toDouble(), 'f', 2) + " kWh</p>";

        // Price Statistics
        html += "<p><strong>💰 Coût total :</strong> " +
                QString::number(statsQuery.value("total_prix").toDouble(), 'f', 2) + " DT</p>";
        html += "<p><strong>📈 Coût moyen :</strong> " +
                QString::number(statsQuery.value("prix_moyen").toDouble(), 'f', 2) + " DT</p>";
        html += QString("<p><strong>⬇️ Coût minimum :</strong> %1 DT</p>")
                    .arg(statsQuery.value("prix_min").toDouble(), 0, 'f', 2);
        html += QString("<p><strong>⬆️ Coût maximum :</strong> %1 DT</p>")
                    .arg(statsQuery.value("prix_max").toDouble(), 0, 'f', 2);

        html += "</div>";
    }

    // Detailed Resources List
    html += "<h2>📋 Liste Détailée des Ressources</h2>";

    // Group by type
    QSqlQuery typeQuery;
    typeQuery.prepare("SELECT DISTINCT TYPE FROM RESSOURCES ORDER BY TYPE");

    if (typeQuery.exec()) {
        while (typeQuery.next()) {
            QString type = typeQuery.value("TYPE").toString();

            html += "<div class='section-title'>Type : " + type + "</div>";
            html += "<table>";
            html += "<tr>";
            html += "<th>ID</th>";
            html += "<th>Localisation</th>";
            html += "<th>Consommation</th>";
            html += "<th>Date de mesure</th>";
            html += "<th>Prix (DT)</th>";
            html += "</tr>";

            QSqlQuery resourcesQuery;
            resourcesQuery.prepare("SELECT ID, LOCALISATION, CONSOMMATION, "
                                   "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY'), PRIX "
                                   "FROM RESSOURCES WHERE TYPE = ? ORDER BY ID");
            resourcesQuery.bindValue(0, type);

            double typeTotal = 0;
            int typeCount = 0;

            if (resourcesQuery.exec()) {
                while (resourcesQuery.next()) {
                    typeCount++;
                    double prix = resourcesQuery.value(4).toDouble();
                    typeTotal += prix;

                    html += "<tr>";
                    html += "<td>" + resourcesQuery.value(0).toString() + "</td>";
                    html += "<td>" + resourcesQuery.value(1).toString() + "</td>";
                    html += "<td style='text-align: center;'>" + resourcesQuery.value(2).toString() + "</td>";
                    html += "<td style='text-align: center;'>" + resourcesQuery.value(3).toString() + "</td>";
                    html += "<td style='text-align: right;'>" + QString::number(prix, 'f', 2) + " DT</td>";
                    html += "</tr>";
                }

                // Type summary row
                if (typeCount > 0) {
                    html += "<tr class='total-row'>";
                    html += "<td colspan='4'><strong>Total pour " + type + " :</strong></td>";
                    html += "<td style='text-align: right;'><strong>" +
                            QString::number(typeTotal, 'f', 2) + " DT</strong></td>";
                    html += "</tr>";
                }
            }

            html += "</table><br>";
        }
    }

    // Overall Summary
    html += "<h2>🎯 Résumé Final</h2>";
    html += "<div class='summary-box' style='background-color: #d4edda; border-left: 5px solid #28a745;'>";

    QSqlQuery finalQuery;
    finalQuery.prepare("SELECT "
                       "COUNT(*) as total, "
                       "SUM(CONSOMMATION) as total_cons, "
                       "SUM(PRIX) as total_prix, "
                       "AVG(PRIX) as avg_prix "
                       "FROM RESSOURCES");

    if (finalQuery.exec() && finalQuery.next()) {
        html += "<p style='color: #155724;'><strong>✅ Rapport complet généré avec succès !</strong></p>";
        html += QString("<p><strong>📈 Efficacité moyenne :</strong> %1 DT/kWh</p>")
                    .arg(finalQuery.value("total_cons").toDouble() > 0 ?
                             QString::number(finalQuery.value("total_prix").toDouble() / finalQuery.value("total_cons").toDouble(), 'f', 3) : "0.000");
        html += QString("<p><strong>📅 Dernière mise à jour :</strong> %1</p>")
                    .arg(currentTime.toString("dd/MM/yyyy à HH:mm"));
    }

    html += "</div>";
    html += "</body></html>";

    document.setHtml(html);

    // Configure printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // Add document properties
    document.setDocumentMargin(20);

    // Generate PDF
    document.print(&printer);

    // Show success message with file info
    QMessageBox::information(this, "✅ Succès",
                             QString("📄 Rapport PDF avancé généré avec succès !\n\n"
                                     "📍 Emplacement : %1\n"
                                     "📏 Pages : 1\n"
                                     "🕐 Généré le : %2")
                                 .arg(fileName)
                                 .arg(currentTime.toString("dd/MM/yyyy HH:mm")));
}
void MainWindow::on_btn_qrcode_clicked()
{
    QString idText = ui->lineEdit_id->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir un ID de ressource !");
        return;
    }

    bool ok;
    int resourceId = idText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "ID invalide !");
        return;
    }

    // Get resource info
    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                  "FROM RESSOURCES WHERE ID = ?");
    query.bindValue(0, resourceId);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Erreur", "Ressource non trouvée !");
        return;
    }

    // Create QR code data
    QString qrData = QString(
                         "=== INFORMATION RESSOURCE ===\n"
                         "ID: %1\n"
                         "Type: %2\n"
                         "Localisation: %3\n"
                         "Consommation: %4\n"
                         "Date: %5\n"
                         "Prix: %6 DT\n"
                         "Généré le: %7")
                         .arg(query.value("ID").toString())
                         .arg(query.value("TYPE").toString())
                         .arg(query.value("LOCALISATION").toString())
                         .arg(query.value("CONSOMMATION").toString())
                         .arg(query.value("DATE_MESURE").toString())
                         .arg(QString::number(query.value("PRIX").toDouble(), 'f', 2))
                         .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    try {
        // Generate QR code
        QrCode qr = QrCode::encodeText(qrData.toUtf8().constData(), QrCode::Ecc::MEDIUM);

        // Create QImage from QR code
        int size = qr.getSize();
        int border = 4;
        QImage image(size + border * 2, size + border * 2, QImage::Format_RGB32);
        image.fill(Qt::white);

        for (int y = 0; y < size; y++) {
            for (int x = 0; x < size; x++) {
                if (qr.getModule(x, y)) {
                    image.setPixel(x + border, y + border, qRgb(0, 0, 0));
                }
            }
        }

        // Scale up the image for better visibility
        image = image.scaled(300, 300, Qt::KeepAspectRatio, Qt::FastTransformation);
        QDialog *qrDialog = new QDialog(this);
        qrDialog->setWindowTitle(QString("QR Code - Ressource ID: %1").arg(resourceId));
        qrDialog->setWindowFlags(qrDialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        qrDialog->setFixedSize(400, 500);

        QVBoxLayout *layout = new QVBoxLayout(qrDialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(15);

        // Title
        QLabel *titleLabel = new QLabel(QString("<h3>QR Code - Ressource ID: %1</h3>").arg(resourceId));
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);

        // Save QR code
        // QR Code Image
        QLabel *qrLabel = new QLabel();
        qrLabel->setPixmap(QPixmap::fromImage(image));
        qrLabel->setAlignment(Qt::AlignCenter);
        qrLabel->setStyleSheet("border: 2px solid #3498db; border-radius: 10px; padding: 10px;");
        layout->addWidget(qrLabel);

        // Resource Information
        QGroupBox *infoGroup = new QGroupBox("Informations de la Ressource");
        infoGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

        QVBoxLayout *infoLayout = new QVBoxLayout();

        QStringList infoLabels;
        infoLabels << QString("Type: %1").arg(query.value("TYPE").toString());
        infoLabels << QString("Localisation: %1").arg(query.value("LOCALISATION").toString());
        infoLabels << QString("Consommation: %1").arg(query.value("CONSOMMATION").toString());
        infoLabels << QString("Date: %1").arg(query.value("DATE_MESURE").toString());
        infoLabels << QString("Prix: %1 DT").arg(QString::number(query.value("PRIX").toDouble(), 'f', 2));

        for (const QString &info : infoLabels) {
            QLabel *label = new QLabel(info);
            label->setWordWrap(true);
            infoLayout->addWidget(label);
        }

        infoGroup->setLayout(infoLayout);
        layout->addWidget(infoGroup);

        // Buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QPushButton *saveButton = new QPushButton("Enregistrer QR Code");
        saveButton->setIcon(QIcon(":/images/save.png"));
        saveButton->setStyleSheet(
            "QPushButton {"
            "   padding: 8px 15px;"
            "   background-color: #5cb4f2;"
            "   color: white;"
            "   border-radius: 5px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #4ca0e0;"
            "}"
            );

        QPushButton *closeButton = new QPushButton("Fermer");
        closeButton->setStyleSheet(
            "QPushButton {"
            "   padding: 8px 15px;"
            "   background-color: #95a5a6;"
            "   color: white;"
            "   border-radius: 5px;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #7f8c8d;"
            "}"
            );

        buttonLayout->addStretch();
        buttonLayout->addWidget(saveButton);
        buttonLayout->addWidget(closeButton);
        buttonLayout->addStretch();

        layout->addLayout(buttonLayout);

        // Connect save button
        connect(saveButton, &QPushButton::clicked, [=]() {
            QString fileName = QFileDialog::getSaveFileName(
                qrDialog,
                "Enregistrer le QR Code",
                QString("qr_code_ressource_%1.png").arg(resourceId),
                "Images PNG (*.png);;Images JPEG (*.jpg *.jpeg);;Images BMP (*.bmp)"
                );

            if (!fileName.isEmpty()) {
                // Ensure file has correct extension
                QString suffix = QFileInfo(fileName).suffix().toLower();
                if (suffix != "png" && suffix != "jpg" && suffix != "jpeg" && suffix != "bmp") {
                    fileName += ".png";
                }

                if (image.save(fileName)) {
                    QMessageBox::information(qrDialog, "Succès",
                                             QString("QR Code enregistré avec succès !\n\n"
                                                     "Fichier: %1")
                                                 .arg(QFileInfo(fileName).fileName()));
                } else {
                    QMessageBox::warning(qrDialog, "Erreur",
                                         "Impossible d'enregistrer le fichier !\n"
                                         "Vérifiez les permissions d'écriture.");
                }
            }
        });

        // Connect close button
        connect(closeButton, &QPushButton::clicked, qrDialog, &QDialog::accept);

        // Show dialog
        qrDialog->exec();
        delete qrDialog;

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Erreur QR Code",
                              QString("Erreur lors de la génération du QR Code:\n%1").arg(e.what()));
    }
}
