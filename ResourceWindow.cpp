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

    // Connect new buttons
    connect(ui->btn_afficher_id, &QPushButton::clicked, this, &ResourceWindow::on_btn_afficher_id_clicked);
    connect(ui->btn_afficher_prix, &QPushButton::clicked, this, &ResourceWindow::on_btn_afficher_prix_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &ResourceWindow::on_btn_pdf_clicked);

    // Connect table click
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &ResourceWindow::on_tableWidget_cellClicked);

    // Initialize
    createResourceTable();
    afficherTable();
    afficherStatistiques(); // Auto-load statistics
}

ResourceWindow::~ResourceWindow()
{
    delete ui;
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
        "   PRIX NUMBER"
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

    Resource r(id, type, localisation, consommation, date_mesure, prix);

    if (r.ajouter()) {
        QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès !");
        afficherTable();
        afficherStatistiques(); // Update statistics
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
            afficherStatistiques(); // Update statistics
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
        afficherStatistiques(); // Update statistics
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
        // Fill the form with found data
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

    // Add summary
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
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF",
                                                    "rapport_ressources.pdf",
                                                    "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument document;
    QString html;

    // HTML header
    html = "<html><head><style>"
           "body { font-family: Arial, sans-serif; }"
           "h1 { color: #2c3e50; text-align: center; }"
           "h2 { color: #34495e; border-bottom: 1px solid #bdc3c7; }"
           "table { width: 100%; border-collapse: collapse; margin: 20px 0; }"
           "th { background-color: #3498db; color: white; padding: 10px; text-align: left; }"
           "td { padding: 8px; border: 1px solid #ddd; }"
           ".summary { background-color: #ecf0f1; padding: 15px; margin: 10px 0; }"
           "</style></head><body>";

    html += "<h1>Rapport des Ressources</h1>";
    html += "<p>Généré le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm") + "</p>";

    // Statistics section
    html += "<h2>Statistiques</h2>";

    QSqlQuery statsQuery;
    statsQuery.prepare("SELECT "
                       "COUNT(*) as total, "
                       "SUM(PRIX) as total_prix, "
                       "AVG(PRIX) as moyenne_prix, "
                       "SUM(CASE WHEN TYPE = 'Eau' THEN 1 ELSE 0 END) as count_eau, "
                       "SUM(CASE WHEN TYPE = 'Electricite' THEN 1 ELSE 0 END) as count_elec "
                       "FROM RESSOURCES");

    if (statsQuery.exec() && statsQuery.next()) {
        int total = statsQuery.value("total").toInt();
        double totalPrix = statsQuery.value("total_prix").toDouble();
        double moyennePrix = statsQuery.value("moyenne_prix").toDouble();
        int countEau = statsQuery.value("count_eau").toInt();
        int countElec = statsQuery.value("count_elec").toInt();

        html += "<div class='summary'>";
        html += "<p><strong>Total des ressources:</strong> " + QString::number(total) + "</p>";
        html += "<p><strong>Prix total:</strong> " + QString::number(totalPrix, 'f', 2) + " DT</p>";
        html += "<p><strong>Prix moyen:</strong> " + QString::number(moyennePrix, 'f', 2) + " DT</p>";
        html += "<p><strong>Ressources Eau:</strong> " + QString::number(countEau) + "</p>";
        html += "<p><strong>Ressources Électricité:</strong> " + QString::number(countElec) + "</p>";
        html += "</div>";
    }

    // Data table section
    html += "<h2>Liste des Ressources</h2>";
    html += "<table>";
    html += "<tr><th>ID</th><th>Type</th><th>Localisation</th><th>Consommation</th><th>Date</th><th>Prix (DT)</th></tr>";

    QSqlQuery dataQuery;
    dataQuery.prepare("SELECT ID, TYPE, LOCALISATION, CONSOMMATION, "
                      "TO_CHAR(DATE_MESURE, 'DD/MM/YYYY') AS DATE_MESURE, PRIX "
                      "FROM RESSOURCES ORDER BY ID");

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

    html += "</table>";
    html += "</body></html>";

    document.setHtml(html);
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

    ui->tableWidget_2->setRowCount(0);  // Fixed: use tableWidget_2 for statistics
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

    ui->tableWidget_2->resizeColumnsToContents();  // Fixed syntax
}
