#include "resourcewindow.h"
#include "ui_resourcewindow.h"
#include "resource.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>
#include <QDate>
#include <QListWidgetItem>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include <QDateTime>
#include <QPainter>
#include <QColor>
#include <QMarginsF>
#include <QPageLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include "StatWindow.h"


// Include real QR code library
#include "libs/qrcodegen/QrCode.hpp"
#include <stdexcept>
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

ResourceWindow::ResourceWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Gestion des Ressources");

    // Connect buttons
    connect(ui->btn_ajouter, &QPushButton::clicked, this, &ResourceWindow::on_btn_ajouter_clicked);
    connect(ui->btn_supprimer, &QPushButton::clicked, this, &ResourceWindow::on_btn_supprimer_clicked);
    connect(ui->btn_modifier, &QPushButton::clicked, this, &ResourceWindow::on_btn_modifier_clicked);
    connect(ui->btn_afficher, &QPushButton::clicked, this, &ResourceWindow::on_btn_afficher_clicked);
    connect(ui->btn_recherche, &QPushButton::clicked, this, &ResourceWindow::on_btn_recherche_clicked);
    connect(ui->btn_pdf_avance, &QPushButton::clicked, this, &ResourceWindow::on_btn_pdf_avance_clicked);

    // Connect new buttons
    connect(ui->btn_afficher_id, &QPushButton::clicked, this, &ResourceWindow::on_btn_afficher_id_clicked);
    connect(ui->btn_afficher_prix, &QPushButton::clicked, this, &ResourceWindow::on_btn_afficher_prix_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &ResourceWindow::on_btn_pdf_clicked);


    // Connect table click
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &ResourceWindow::on_tableWidget_cellClicked);

    // Initialize
    createResourceTable();
    afficherTable();
    afficherStatistiques();
}

ResourceWindow::~ResourceWindow()
{
    delete ui;
}
void ResourceWindow::on_statsButton_clicked()
{
    StatWindow *statsWindow = new StatWindow(this);
    statsWindow->setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed
    statsWindow->show();
    statsWindow->raise();
    statsWindow->activateWindow();
}
bool ResourceWindow::createResourceTable()
{
    QSqlQuery query;
    QString createTableSQL =
        "CREATE TABLE RESSOURCES ("
        "   ID NUMBER PRIMARY KEY, "
        "   TYPE VARCHAR2(100), "
        "   LOCALISATION VARCHAR2(100), "
        "   CONSOMMATION NUMBER, "
        "   DATE_MESURE DATE, "
        "   PRIX NUMBER, "
        "   RESIDENT_ID NUMBER"
        ")";

    if (!query.exec(createTableSQL)) {
        QSqlError error = query.lastError();
        if (!error.databaseText().contains("ORA-00955")) {
            qDebug() << "Table creation warning:" << error.text();
        }
    }
    return true;
}

QString ResourceWindow::getSelectedType() const
{
    if (ui->radio_eau->isChecked())
        return "Eau";
    else if (ui->radio_elec->isChecked())
        return "Electricite";
    else
        return "";
}

void ResourceWindow::afficherTable()
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

// FONCTION POUR OBTENIR UN RÉSIDENT PAR DÉFAUT
int ResourceWindow::getDefaultResidentId()
{
    QSqlQuery query;
    query.prepare("SELECT ID FROM RESIDENT WHERE ROWNUM = 1 ORDER BY ID");

    if (query.exec() && query.next()) {
        return query.value("ID").toInt();
    }

    // Si aucun résident n'existe, retourner -1
    return -1;
}

void ResourceWindow::on_btn_ajouter_clicked()
{
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

    // OBTENIR LE RÉSIDENT PAR DÉFAUT
    int residentId = getDefaultResidentId();

    if (residentId == -1) {
        QMessageBox::warning(this, "Erreur", "Aucun résident trouvé dans la base de données !");
        return;
    }

    Resource r(id, type, localisation, consommation, date_mesure, prix);
    r.setResidentId(residentId); // SET LE RESIDENT_ID

    if (r.ajouter()) {
        QMessageBox::information(this, "Succès",
                                 QString("Ressource ajoutée avec succès !\nAssignée automatiquement au résident ID: %1").arg(residentId));
        afficherTable();
        afficherStatistiques();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de la ressource !");
    }
}

void ResourceWindow::on_btn_supprimer_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID à supprimer !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer la ressource ID " + QString::number(id) + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Resource r;
        if (r.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Ressource supprimée !");
            afficherTable();
            afficherStatistiques();
            clearInputs();
        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la suppression ! ID non trouvé.");
        }
    }
}

void ResourceWindow::on_btn_modifier_clicked()
{
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
        afficherStatistiques();
        clearInputs();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification ! ID non trouvé.");
    }
}

void ResourceWindow::on_btn_afficher_clicked()
{
    afficherTable();
}

void ResourceWindow::on_btn_recherche_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID à rechercher !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    chargerDonneesParID(id);
}

void ResourceWindow::chargerDonneesParID(int id)
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

void ResourceWindow::clearInputs()
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

void ResourceWindow::on_tableWidget_cellClicked(int row, int column)
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
        QString dateStr = ui->tableWidget->item(row, 4)->text();
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (date.isValid()) {
            ui->dateEdit_mesure->setDate(date);
        }
    }
}

void ResourceWindow::on_btn_afficher_id_clicked()
{
    if (ui->lineEdit_id->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID à afficher !");
        return;
    }

    int id = ui->lineEdit_id->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                  "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                  "FROM RESSOURCES WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'affichage: " + error.text());
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

    if (row == 0) {
        QMessageBox::information(this, "Information", "Aucune ressource trouvée avec cet ID.");
    }

    ui->tableWidget->resizeColumnsToContents();
}

void ResourceWindow::on_btn_afficher_prix_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT ID, TYPE, PRIX FROM RESSOURCES ORDER BY ID");

    if (!query.exec()) {
        QSqlError error = query.lastError();
        QMessageBox::warning(this, "Erreur", "Erreur lors du chargement des prix: " + error.text());
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

void ResourceWindow::on_btn_pdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "rapport_ressources.pdf", "PDF Files (*.pdf)");
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
    dataQuery.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX FROM RESSOURCES ORDER BY ID");

    if (dataQuery.exec()) {
        while (dataQuery.next()) {
            html += "<tr>";
            html += "<td>" + dataQuery.value("ID").toString() + "</td>";
            html += "<td>" + dataQuery.value("TYPE").toString() + "</td>";
            html += "<td>" + dataQuery.value("LOCALISATION").toString() + "</td>";
            html += "<td>" + dataQuery.value("CONSOMMATION").toString() + "</td>";
            html += "<td>" + dataQuery.value("DATE_MESURE").toString() + "</td>";
            html += "<td>" + QString::number(dataQuery.value("PRIX").toDouble(), 'f', 2) + "</td>";
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

void ResourceWindow::afficherStatistiques()
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

void ResourceWindow::on_btn_pdf_avance_clicked()
{
    genererPDFAvance();
}

void ResourceWindow::genererPDFAvance()
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

    QString debugInfo = "=== DEBUG FINAL ===\n\n";

    // Test jointure pour cet ID uniquement
    QSqlQuery jointureQuery;
    jointureQuery.prepare("SELECT "
                          "r.ID as res_id, "
                          "r.RESIDENT_ID as res_resident_id, "
                          "res.ID as resident_id, "
                          "res.FIRST_NAME as first_name, "
                          "res.LAST_NAME as last_name "
                          "FROM RESSOURCES r "
                          "JOIN RESIDENT res ON r.RESIDENT_ID = res.ID "
                          "WHERE res.ID = :id");
    jointureQuery.bindValue(":id", residentIdInput);

    debugInfo += "1. Test jointure pour l'ID saisi:\n";
    if (jointureQuery.exec()) {
        int count = 0;
        while (jointureQuery.next()) {
            count++;
            debugInfo += QString("   Ligne %1:\n").arg(count);
            debugInfo += QString("   - Resource ID: %1\n").arg(jointureQuery.value("res_id").toString());
            debugInfo += QString("   - RESIDENT_ID: %1\n").arg(jointureQuery.value("res_resident_id").toString());
            debugInfo += QString("   - Resident table ID: %1\n").arg(jointureQuery.value("resident_id").toString());
            debugInfo += QString("   - Prénom: %1\n").arg(jointureQuery.value("first_name").toString());
            debugInfo += QString("   - Nom: %1\n").arg(jointureQuery.value("last_name").toString());
        }
        debugInfo += QString("   Total: %1 lignes\n\n").arg(count);
    } else {
        debugInfo += "   ERREUR: " + jointureQuery.lastError().text() + "\n\n";
    }

    QMessageBox::information(this, "Debug Final", debugInfo);

    // Générer le PDF pour cet ID uniquement
    genererPDFGaranti(residentIdInput);
}

void ResourceWindow::genererPDFGaranti(int residentIdInput)
{
    QString fileName = QFileDialog::getSaveFileName(this, "PDF Garanti", "rapport_garanti.pdf", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    QString html = "<html><body style='font-family: Arial; margin: 20px;'>";
    html += "<h1>Rapport Garanti</h1>";
    html += "<p><strong>Généré le:</strong> " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";

    // Récupérer le résident
    QSqlQuery residentQuery;
    residentQuery.prepare("SELECT FIRST_NAME, LAST_NAME FROM RESIDENT WHERE ID = :id");
    residentQuery.bindValue(":id", residentIdInput);

    QString residentName = "Résident inconnu";
    if (residentQuery.exec() && residentQuery.next()) {
        residentName = residentQuery.value("FIRST_NAME").toString() + " " + residentQuery.value("LAST_NAME").toString();
    }

    // Récupérer les ressources de ce résident uniquement
    QSqlQuery ressourcesQuery;
    ressourcesQuery.prepare("SELECT TYPE, LOCALISATION, CONSOMMATION, PRIX FROM RESSOURCES WHERE RESIDENT_ID = :id");
    ressourcesQuery.bindValue(":id", residentIdInput);

    html += "<h2>Informations du résident : " + residentName + "</h2>";
    html += "<table border='1' style='border-collapse: collapse; width: 100%;'>";
    html += "<tr style='background-color: #3498db; color: white;'>";
    html += "<th style='padding: 10px;'>Type</th><th>Localisation</th><th>Consommation</th><th>Prix</th>";
    html += "</tr>";

    int count = 0;
    if (ressourcesQuery.exec()) {
        while (ressourcesQuery.next()) {
            count++;
            html += "<tr>";
            html += "<td style='padding: 8px;'>" + ressourcesQuery.value("TYPE").toString() + "</td>";
            html += "<td style='padding: 8px;'>" + ressourcesQuery.value("LOCALISATION").toString() + "</td>";
            html += "<td style='padding: 8px; text-align: center;'>" + ressourcesQuery.value("CONSOMMATION").toString() + "</td>";
            html += "<td style='padding: 8px; text-align: right;'>" + ressourcesQuery.value("PRIX").toString() + " DT</td>";
            html += "</tr>";
        }
    }

    html += "</table>";

    if (count > 0) {
        html += "<p style='color: green; font-weight: bold;'>✅ " + QString::number(count) + " ressources affichées</p>";
    } else {
        html += "<p style='color: red; font-weight: bold;'>❌ Aucune ressource pour cet ID</p>";
    }

    html += "</body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument document;
    document.setHtml(html);
    document.print(&printer);

    QMessageBox::information(this, "Résultat Final",
                             QString("PDF généré\nRessources affichées: %1\nFichier: %2").arg(count).arg(fileName));
}

// ---------------- QR CODE pour un ID ----------------
void ResourceWindow::showQRCodeDialogForID()
{
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

    // Récupérer le résident
    QSqlQuery residentQuery;
    residentQuery.prepare("SELECT FIRST_NAME, LAST_NAME FROM RESIDENT WHERE ID = :id");
    residentQuery.bindValue(":id", residentIdInput);

    QString residentName = "Résident inconnu";
    if (residentQuery.exec() && residentQuery.next()) {
        residentName = residentQuery.value("FIRST_NAME").toString() + " " + residentQuery.value("LAST_NAME").toString();
    }

    QString qrData = "=== RAPPORT DU RESIDENT ===\n";
    qrData += "ID: " + QString::number(residentIdInput) + "\n";
    qrData += "Nom: " + residentName + "\n\n";

    QSqlQuery ressourcesQuery;
    ressourcesQuery.prepare("SELECT TYPE, LOCALISATION, CONSOMMATION, DATE_MESURE, PRIX FROM RESSOURCES WHERE RESIDENT_ID = :id");
    ressourcesQuery.bindValue(":id", residentIdInput);

    if (ressourcesQuery.exec()) {
        int count = 0;
        while (ressourcesQuery.next()) {
            count++;
            qrData += QString("%1 | %2 | %3 | %4 | %5 DT\n")
                          .arg(ressourcesQuery.value("TYPE").toString())
                          .arg(ressourcesQuery.value("LOCALISATION").toString())
                          .arg(ressourcesQuery.value("CONSOMMATION").toString())
                          .arg(ressourcesQuery.value("DATE_MESURE").toDate().toString("dd/MM/yyyy"))
                          .arg(ressourcesQuery.value("PRIX").toString());
        }

        if (count == 0) qrData += "Aucune ressource pour cet ID\n";
    }

    qrData += "\n=== Généré par EcoVision System ===";

    // Générer le QR
    QDialog qrDialog(this);
    qrDialog.setWindowTitle("Code QR - Rapport du Résident");
    qrDialog.setFixedSize(400, 500);

    QVBoxLayout* layout = new QVBoxLayout(&qrDialog);
    QLabel* titleLabel = new QLabel("Rapport du Résident", &qrDialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont("Arial", 14, QFont::Bold);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    QrCode qrCode = QrCode::encodeText(qrData.toUtf8().constData(), QrCode::Ecc::MEDIUM);

    QLabel* qrLabel = new QLabel(&qrDialog);
    qrLabel->setAlignment(Qt::AlignCenter);

    const int qrSize = 300;
    int qrModuleSize = qrCode.getSize();
    int cellSize = qrSize / qrModuleSize;

    QPixmap qrPixmap(qrSize, qrSize);
    qrPixmap.fill(Qt::white);
    QPainter painter(&qrPixmap);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    for (int y = 0; y < qrModuleSize; y++) {
        for (int x = 0; x < qrModuleSize; x++) {
            if (qrCode.getModule(x, y)) {
                painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
            }
        }
    }

    qrLabel->setPixmap(qrPixmap);
    layout->addWidget(qrLabel);

    QPushButton* closeBtn = new QPushButton("Fermer", &qrDialog);
    connect(closeBtn, &QPushButton::clicked, &qrDialog, &QDialog::accept);
    layout->addWidget(closeBtn);

    qrDialog.exec();
}

void ResourceWindow::on_btn_qrcode_clicked()
{
    showQRCodeDialogForID();
}
