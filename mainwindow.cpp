#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    codeSelectionne = -1;

    // ========== Configuration du TableWidget ==========
    ui->tableWidget->setColumnCount(6);
    QStringList headers;
    headers << "Code" << "Date Réclamation" << "Nom Résident" << "Description" << "État" << "Délai";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // ========== Configuration du TreeWidget ==========
    ui->treeWidget->setColumnCount(5);
    QStringList treeHeaders;
    treeHeaders << "Nom du résident" << "Date" << "Description" << "État" << "Code";
    ui->treeWidget->setHeaderLabels(treeHeaders);
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // ========== Initialiser le ComboBox État ==========
    ui->comboBox->clear();
    ui->comboBox->addItem("traité");
    ui->comboBox->addItem("non traité");

    afficherReclamations();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afficherReclamations()
{
    QSqlQuery query;
    query.prepare("SELECT CODE, DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI FROM RECLAMATION ORDER BY CODE DESC");

    if(query.exec())
    {
        ui->tableWidget->setRowCount(0);
        int row = 0;

        while(query.next())
        {
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toDate().toString("dd/MM/yyyy")));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(query.value(4).toString()));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(query.value(5).toString()));
            row++;
        }
        ui->tableWidget->resizeColumnsToContents();
    }
    else
    {
        QMessageBox::warning(nullptr, "Erreur", "Erreur lors de l'affichage : " + query.lastError().text());
    }
}
// ==================== AJOUTER (pushButton_9) ====================
void MainWindow::on_pushButton_9_clicked()
{
    QString date_str = ui->lineEdit_2->text().trimmed();
    QString nom = ui->lineEdit_3->text().trimmed();
    QString description = ui->lineEdit_4->text().trimmed();
    QString etat = ui->lineEdit_5->text().trimmed().toLower();
    QString delai = ui->lineEdit_6->text().trimmed();

    // ===== VALIDATION : CHAMPS VIDES =====
    if(date_str.isEmpty() || nom.isEmpty() || description.isEmpty() ||
        etat.isEmpty() || delai.isEmpty())
    {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs !");
        return;
    }

    // ===== CONTRÔLE DE SAISIE : DATE =====
    QDate date = QDate::fromString(date_str, "dd/MM/yyyy");
    if(!date.isValid())
    {
        QMessageBox::warning(this, "Date invalide",
                             "Format attendu : dd/MM/yyyy\n"
                             "Exemple : 10/11/2025");
        ui->lineEdit_2->setFocus();
        ui->lineEdit_2->selectAll();
        return;
    }

    // ===== CONTRÔLE DE SAISIE : ÉTAT =====
    if(etat != "traité" && etat != "non traité")
    {
        QMessageBox::warning(this, "État invalide",
                             "L'état doit être exactement :\n"
                             "• traité\n"
                             "• non traité\n\n"
                             "(en minuscules, avec accent)");
        ui->lineEdit_5->setFocus();
        ui->lineEdit_5->selectAll();
        return;
    }

    // ===== AJOUT DANS LA BASE =====
    Reclamation r(0, date, nom, description, etat, delai);

    if(r.ajouter())
    {
        QMessageBox::information(this, "Succès",
                                 "Réclamation ajoutée avec succès !");

        // Vider tous les champs
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        ui->lineEdit_6->clear();

        afficherReclamations();

        // Retourner vers l'accueil
        ui->tabWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::critical(this, "Erreur",
                              "Échec de l'ajout dans la base de données !");
    }
}
// ==================== SUPPRIMER ====================

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    codeSelectionne = ui->tableWidget->item(row, 0)->text().toInt();
}

void MainWindow::on_pushButton_12_clicked()
{
    if (codeSelectionne == -1)
    {
        QMessageBox::warning(this, "Aucune sélection",
                             "Veuillez d'abord sélectionner une réclamation dans le tableau !");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cette réclamation ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        Reclamation r;
        if (r.supprimer(codeSelectionne))
        {
            QMessageBox::information(this, "Succès", "Réclamation supprimée avec succès !");
            afficherReclamations();
            codeSelectionne = -1;
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué !");
        }
    }
}
// ==================== RECHERCHE AUTO (lineEdit_7) ====================
void MainWindow::on_lineEdit_7_textChanged(const QString &text)
{
    if(text.isEmpty())
    {
        ui->lineEdit_8->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->lineEdit_11->clear();
        return;
    }

    bool ok;
    int code = text.toInt(&ok);

    if(ok)
    {
        Reclamation r = Reclamation::rechercherParCode(code);

        if(r.getCode() != 0)
        {
            ui->lineEdit_8->setText(r.getDateReclamation().toString("dd/MM/yyyy"));
            ui->lineEdit_9->setText(r.getNomResident());
            ui->lineEdit_10->setText(r.getDescription());

            QString etat = r.getEtat();
            if(etat == "traité")
                ui->comboBox->setCurrentIndex(0);
            else if(etat == "non traité")
                ui->comboBox->setCurrentIndex(1);

            ui->lineEdit_11->setText(r.getDelai());
        }
        else
        {
            ui->lineEdit_8->clear();
            ui->lineEdit_9->clear();
            ui->lineEdit_10->clear();
            ui->comboBox->setCurrentIndex(0);
            ui->lineEdit_11->clear();
        }
    }
}

// ==================== ENREGISTRER MODIFICATION (pushButton_11) ====================
void MainWindow::on_pushButton_11_clicked()
{
    QString code_str = ui->lineEdit_7->text().trimmed();
    QString date_str = ui->lineEdit_8->text().trimmed();
    QString nom = ui->lineEdit_9->text().trimmed();
    QString description = ui->lineEdit_10->text().trimmed();
    QString etat = ui->comboBox->currentText().toLower();
    QString delai = ui->lineEdit_11->text().trimmed();

    // ===== VALIDATION : CODE =====
    if(code_str.isEmpty())
    {
        QMessageBox::warning(this, "Code manquant",
                             "Veuillez entrer un code de réclamation !");
        ui->lineEdit_7->setFocus();
        return;
    }

    bool ok;
    int code = code_str.toInt(&ok);
    if(!ok)
    {
        QMessageBox::warning(this, "Code invalide",
                             "Le code doit être un nombre entier !");
        ui->lineEdit_7->setFocus();
        ui->lineEdit_7->selectAll();
        return;
    }

    Reclamation test = Reclamation::rechercherParCode(code);
    if(test.getCode() == 0)
    {
        QMessageBox::warning(this, "Code inexistant",
                             "Aucune réclamation trouvée avec ce code !");
        return;
    }

    // ===== VALIDATION : CHAMPS VIDES =====
    if(date_str.isEmpty() || nom.isEmpty() || description.isEmpty() ||
        etat.isEmpty() || delai.isEmpty())
    {
        QMessageBox::warning(this, "Champs manquants",
                             "Veuillez remplir tous les champs !");
        return;
    }

    // ===== CONTRÔLE DE SAISIE : DATE =====
    QDate date = QDate::fromString(date_str, "dd/MM/yyyy");
    if(!date.isValid())
    {
        QMessageBox::warning(this, "Date invalide",
                             "Format attendu : dd/MM/yyyy\n"
                             "Exemple : 10/11/2025");
        ui->lineEdit_8->setFocus();
        ui->lineEdit_8->selectAll();
        return;
    }

    // ===== MODIFICATION DANS LA BASE =====
    Reclamation r(code, date, nom, description, etat, delai);

    if(r.modifier(code))
    {
        QMessageBox::information(this, "Succès",
                                 "Réclamation modifiée avec succès !");

        ui->lineEdit_7->clear();
        ui->lineEdit_8->clear();
        ui->lineEdit_9->clear();
        ui->lineEdit_10->clear();
        ui->comboBox->setCurrentIndex(0);
        ui->lineEdit_11->clear();

        afficherReclamations();

        // Retourner vers l'accueil
        ui->tabWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::critical(this, "Erreur",
                              "Échec de la modification dans la base de données !");
    }
}
// ==================== ALLER VERS AJOUTER (pushButton_6) ====================
void MainWindow::on_pushButton_6_clicked()
{
    ui->tabWidget->setCurrentIndex(1);  // Aller vers onglet Ajouter
}

// ==================== ALLER VERS MODIFIER (pushButton_7) ====================
void MainWindow::on_pushButton_7_clicked()
{
    ui->tabWidget->setCurrentIndex(2);  // Aller vers onglet Modifier
}
// ==================== CHERCHER PAR DATE (pushButton_8) ====================
void MainWindow::on_pushButton_8_clicked()
{
    QString date_str = ui->lineEdit->text().trimmed();

    if(date_str.isEmpty())
    {
        QMessageBox::warning(this, "Date manquante",
                             "Veuillez entrer une date à rechercher !");
        ui->lineEdit->setFocus();
        return;
    }

    // Valider le format de la date
    QDate date = QDate::fromString(date_str, "dd/MM/yyyy");
    if(!date.isValid())
    {
        QMessageBox::warning(this, "Date invalide",
                             "Format attendu : dd/MM/yyyy\n"
                             "Exemple : 10/11/2025");
        ui->lineEdit->setFocus();
        ui->lineEdit->selectAll();
        return;
    }

    // Rechercher dans la base
    QList<Reclamation> resultats = Reclamation::rechercherParDate(date);

    if(resultats.isEmpty())
    {
        QMessageBox::information(this, "Aucun résultat",
                                 "Aucune réclamation trouvée pour la date : " + date_str);
        ui->treeWidget->clear();
    }
    else
    {
        afficherDansTreeWidget(resultats);
        QMessageBox::information(this, "Résultats",
                                 QString::number(resultats.size()) + " réclamation(s) trouvée(s) !");
    }
}

// ==================== AFFICHER RÉSULTATS DANS TREEWIDGET ====================
void MainWindow::afficherDansTreeWidget(QList<Reclamation> liste)
{
    ui->treeWidget->clear();

    foreach(Reclamation r, liste)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, r.getNomResident());           // Nom du résident
        item->setText(1, r.getDateReclamation().toString("dd/MM/yyyy"));  // Date
        item->setText(2, r.getDescription());           // Description
        item->setText(3, r.getEtat());                  // État
        item->setText(4, QString::number(r.getCode())); // Code

        ui->treeWidget->addTopLevelItem(item);
    }

    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->resizeColumnToContents(2);
    ui->treeWidget->resizeColumnToContents(3);
    ui->treeWidget->resizeColumnToContents(4);
}
