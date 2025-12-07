#include "employewindow.h"
#include "ui_employewindow.h"
#include "employee.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTimer>
#include <QHeaderView>
#include <QDebug>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPageSize>
#include <QDateTime>

employewindow::employewindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)  // Remove currentTabIndex since it's not in header
{
    qDebug() << "=== DEBUG: employewindow constructor START ===";

    try {
        // Setup UI FIRST
        ui->setupUi(this);
        qDebug() << "DEBUG: UI setup completed";

        // Set window title
        setWindowTitle("Gestion des Employés");

        // Basic table configuration
        if (ui->tableWidget) {
            ui->tableWidget->setColumnCount(8);
            ui->tableWidget->setHorizontalHeaderLabels(
                QStringList() << "CIN" << "Nom" << "Prénom" << "Téléphone"
                              << "Adresse" << "Métier" << "Présence" << "Salaire");

            ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
            ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
            ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
            ui->tableWidget->setAlternatingRowColors(true);

            qDebug() << "DEBUG: Table configured";
        } else {
            qDebug() << "ERROR: tableWidget is null!";
        }

        // SAFE button connections
        if (ui->btnAjouter) {
            connect(ui->btnAjouter, &QPushButton::clicked, this, &employewindow::onAddEmploye);
            qDebug() << "DEBUG: btnAjouter connected";
        }

        if (ui->btnSupprimer) {
            connect(ui->btnSupprimer, &QPushButton::clicked, this, &employewindow::onDeleteEmploye);
            qDebug() << "DEBUG: btnSupprimer connected";
        }

        if (ui->btnModifier) {
            connect(ui->btnModifier, &QPushButton::clicked, this, &employewindow::onUpdateEmploye);
            qDebug() << "DEBUG: btnModifier connected";
        }

        if (ui->btntrier) {
            connect(ui->btntrier, &QPushButton::clicked, this, &employewindow::onTrierParSalaire);
            qDebug() << "DEBUG: btntrier connected";
        }

        // Table selection connection
        if (ui->tableWidget) {
            connect(ui->tableWidget, &QTableWidget::cellClicked, this, &employewindow::onTableRowSelected);
            qDebug() << "DEBUG: tableWidget cellClicked connected";
        }

        // PDF button - simple connection
        if (ui->btnpdf) {
            connect(ui->btnpdf, &QPushButton::clicked, this, &employewindow::exportPdf);
            qDebug() << "DEBUG: btnpdf connected";
        }

        // Search - simple connection
        if (ui->cin_4) {
            connect(ui->cin_4, &QLineEdit::textChanged, this, &employewindow::onSearchEmploye);
            qDebug() << "DEBUG: cin_4 search connected";
        }

        qDebug() << "=== DEBUG: employewindow constructor COMPLETED SUCCESSFULLY ===";

    } catch (const std::exception& e) {
        qDebug() << "EXCEPTION in employewindow constructor:" << e.what();
        QMessageBox::critical(nullptr, "Error", QString("Exception: %1").arg(e.what()));
    } catch (...) {
        qDebug() << "UNKNOWN EXCEPTION in employewindow constructor";
        QMessageBox::critical(nullptr, "Error", "Unknown exception occurred");
    }
}

employewindow::~employewindow()
{
    delete ui;
    qDebug() << "DEBUG: employewindow destroyed";
}

bool employewindow::isDigitsOnly(const QString &s)
{
    for (const QChar &ch : s)
        if (!ch.isDigit()) return false;
    return true;
}

bool employewindow::isValidDecimal(const QString &s)
{
    bool ok;
    s.toDouble(&ok);
    return ok;
}

void employewindow::clearFields()
{
    if (ui->cin_2) ui->cin_2->clear();
    if (ui->nom_2) ui->nom_2->clear();
    if (ui->prenom_2) ui->prenom_2->clear();
    if (ui->adresse_2) ui->adresse_2->clear();
    if (ui->telephone_2) ui->telephone_2->clear();
    if (ui->metier_2) ui->metier_2->clear();
    if (ui->presence_2) ui->presence_2->clear();
    if (ui->salaire_2) ui->salaire_2->clear();
    if (ui->cin_2) ui->cin_2->setFocus();
}

void employewindow::onAddEmploye()
{
    qDebug() << "onAddEmploye called";

    // Check database connection
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Erreur", "Base de données non connectée!");
        return;
    }

    const QString cin = ui->cin_2->text().trimmed();
    const QString nom = ui->nom_2->text().trimmed();
    const QString prenom = ui->prenom_2->text().trimmed();
    const QString adresse = ui->adresse_2->text().trimmed();
    const QString telephone = ui->telephone_2->text().trimmed();
    const QString metier = ui->metier_2->text().trimmed();
    const QString presence = ui->presence_2->text().trimmed();
    const QString salaire = ui->salaire_2->text().trimmed();

    if (cin.length() != 8 || !isDigitsOnly(cin)) {
        QMessageBox::warning(this, "Validation", "CIN doit contenir exactement 8 chiffres.");
        return;
    }
    if (nom.isEmpty() || prenom.isEmpty()) {
        QMessageBox::warning(this, "Validation", "Nom et Prénom sont obligatoires.");
        return;
    }
    if (!telephone.isEmpty() && !isDigitsOnly(telephone)) {
        QMessageBox::warning(this, "Validation", "Téléphone doit contenir uniquement des chiffres.");
        return;
    }
    if (!salaire.isEmpty() && !isValidDecimal(salaire)) {
        QMessageBox::warning(this, "Validation", "Salaire doit être un nombre valide.");
        return;
    }

    Employee e(cin, nom, prenom, adresse, telephone, metier, salaire.toDouble(), presence);
    if (!e.addToDatabase()) {
        QMessageBox::critical(this, "Erreur SQL", "Insertion échouée.");
        return;
    }

    clearFields();
    QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
}

void employewindow::onDeleteEmploye()
{
    qDebug() << "onDeleteEmploye called";

    const QString cin = ui->cin_2->text().trimmed();
    if (cin.isEmpty() || cin.length() != 8 || !isDigitsOnly(cin)) {
        QMessageBox::warning(this, "Validation", "Veuillez fournir un CIN valide pour la suppression.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer", "Supprimer cet employé ?") != QMessageBox::Yes)
        return;

    if (!Employee::deleteByCin(cin)) {
        QMessageBox::critical(this, "Erreur SQL", "Suppression échouée.");
        return;
    }

    clearFields();
    QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
}

void employewindow::onUpdateEmploye()
{
    qDebug() << "onUpdateEmploye called";

    const QString cin = ui->cin_2->text().trimmed();
    if (cin.length() != 8 || !isDigitsOnly(cin)) {
        QMessageBox::warning(this, "Validation", "Veuillez fournir un CIN valide.");
        return;
    }

    const QString nom = ui->nom_2->text().trimmed();
    const QString prenom = ui->prenom_2->text().trimmed();
    const QString adresse = ui->adresse_2->text().trimmed();
    const QString telephone = ui->telephone_2->text().trimmed();
    const QString metier = ui->metier_2->text().trimmed();
    const QString presence = ui->presence_2->text().trimmed();
    const QString salaire = ui->salaire_2->text().trimmed();

    if (!salaire.isEmpty() && !isValidDecimal(salaire)) {
        QMessageBox::warning(this, "Validation", "Salaire doit être un nombre valide.");
        return;
    }

    Employee e(cin, nom, prenom, adresse, telephone, metier, salaire.toDouble(), presence);
    if (!e.updateInDatabase()) {
        QMessageBox::critical(this, "Erreur SQL", "Mise à jour échouée.");
        return;
    }

    clearFields();
    QMessageBox::information(this, "Succès", "Employé mis à jour avec succès.");
}

void employewindow::onTableRowSelected(int row, int column)
{
    Q_UNUSED(column);

    qDebug() << "onTableRowSelected called, row:" << row;

    if (row < 0 || row >= ui->tableWidget->rowCount()) return;

    if (QTableWidgetItem *cinItem = ui->tableWidget->item(row, 0)) ui->cin_2->setText(cinItem->text());
    if (QTableWidgetItem *nomItem = ui->tableWidget->item(row, 1)) ui->nom_2->setText(nomItem->text());
    if (QTableWidgetItem *prenomItem = ui->tableWidget->item(row, 2)) ui->prenom_2->setText(prenomItem->text());
    if (QTableWidgetItem *telephoneItem = ui->tableWidget->item(row, 3)) ui->telephone_2->setText(telephoneItem->text());
    if (QTableWidgetItem *adresseItem = ui->tableWidget->item(row, 4)) ui->adresse_2->setText(adresseItem->text());
    if (QTableWidgetItem *metierItem = ui->tableWidget->item(row, 5)) ui->metier_2->setText(metierItem->text());
    if (QTableWidgetItem *presenceItem = ui->tableWidget->item(row, 6)) ui->presence_2->setText(presenceItem->text());
    if (QTableWidgetItem *salaireItem = ui->tableWidget->item(row, 7)) ui->salaire_2->setText(salaireItem->text());
}


void employewindow::onSearchEmploye()
{
    qDebug() << "onSearchEmploye called";

    const QString search = ui->cin_4->text().trimmed();
    if (search.isEmpty()) {
        return;
    }

    QString where = "cin LIKE ?";
    QVariantList binds;
    binds << ("%" + search + "%");

    ui->tableWidget->setRowCount(0);
    QSqlQuery q;
    q.prepare("SELECT cin, nom, prenom, telephone, adresse, metier, presence, salaire FROM EMPLOYEE WHERE " + where);
    q.bindValue(0, binds[0]);

    if (q.exec()) {
        while (q.next()) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(q.value(3).toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(q.value(5).toString()));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(q.value(6).toString()));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(q.value(7).toDouble(), 'f', 2)));
        }
    }

    ui->tableWidget->resizeColumnsToContents();
}

void employewindow::onTrierParSalaire()
{
    QVector<Employee> employes = Employee::fetchAllSortedBySalaire();
    ui->tableWidget->setRowCount(0);

    for (const Employee &emp : employes) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(emp.getCin()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(emp.getNom()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(emp.getPrenom()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(emp.getTelephone()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(emp.getAdresse()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(emp.getMetier()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(emp.getPresence()));
        ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(emp.getSalaire(), 'f', 2)));
    }

    ui->tableWidget->resizeColumnsToContents();
    qDebug() << "Employees sorted by salary";
}

void employewindow::exportPdf()
{
    qDebug() << "exportPdf called";

    // Simple PDF export for now
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fileName);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF.");
        return;
    }

    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(100, 100, "Liste des Employés");

    painter.setFont(QFont("Arial", 10));
    painter.drawText(100, 150, "Exporté le: " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));

    painter.end();

    QMessageBox::information(this, "Succès", "Le PDF a été créé avec succès.");
}
