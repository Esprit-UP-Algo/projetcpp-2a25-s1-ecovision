#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "rfid_controller.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QTimer>
#include <QHeaderView>
#include <QDebug>
#include <QStatusBar>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QPrinter>
#include <QPageSize>
#include <QPainter>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_arduinoManager(nullptr)
    , m_rfidDialog(nullptr)
    , m_rfidController(nullptr)
{
    ui->setupUi(this);

    // ⚠️ RFID initialization will be done later via initializeRFID() after DB connection

    connect(ui->btnAjouter, &QPushButton::clicked, this, &MainWindow::onAddEmploye);
    connect(ui->btnSupprimer, &QPushButton::clicked, this, &MainWindow::onDeleteEmploye);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::onUpdateEmploye);
    connect(ui->tableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTableRowSelected);
    connect(ui->btntrier, &QPushButton::clicked, this, &MainWindow::onTrierParSalaire);
    connect(ui->btnpdf, &QPushButton::clicked, this, &MainWindow::exportPdf);
    // Recherche dynamique
    QTimer *searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    connect(ui->cin_4, &QLineEdit::textChanged, this, [searchTimer](const QString&){ searchTimer->start(300); });
    connect(searchTimer, &QTimer::timeout, this, &MainWindow::onSearchEmploye);

    // Configuration du tableau avec 8 colonnes
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(
        QStringList() << "CIN" << "Nom" << "Prénom" << "Téléphone"
                      << "Adresse" << "Métier" << "Présence" << "Salaire"
        );

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Styling du QTableWidget
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet(R"(
        QTableWidget {
            background-color: #ffffff;
            alternate-background-color: #f6fbff;
            gridline-color: #e6f0fb;
            selection-background-color: #79aacbff;
            selection-color: #012233;
            font-size: 12px;
        }
        QTableWidget::item:selected {
            background-color: #7fb3d5;
            color: #012233;
        }
        QHeaderView::section {
            background-color: #2e86ab;
            color: #ffffff;
            padding: 6px;
            border: 0px;
        }
        QScrollBar:vertical { width: 10px; }
        QScrollBar::handle:vertical { background: #c4dbe8; border-radius: 5px; }
    )");

    QFont hfont = ui->tableWidget->horizontalHeader()->font();
    hfont.setBold(true);
    ui->tableWidget->horizontalHeader()->setFont(hfont);

    // Chercher le QTabWidget dans l'interface
    QTabWidget *tabWidget = findChild<QTabWidget*>();
    if (tabWidget) {
        connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    }

    onDisplayAllEmployes();
}

MainWindow::~MainWindow()
{
    if (m_arduinoManager && m_arduinoManager->isOpen()) {
        m_arduinoManager->close();
    }
    delete ui;
}

// Gérer le changement d'onglet
void MainWindow::onTabChanged(int index)
{
    currentTabIndex = index;
    // Vérifier si l'onglet "Statistiques" est sélectionné
    if (index == 1) {
        updateStatisticsTab();
    }
}

// 🆕 Nouvelle fonction pour rafraîchir automatiquement les stats
void MainWindow::autoRefreshStatistics()
{
    // Si l'onglet statistiques est actuellement visible, le mettre à jour
    if (currentTabIndex == 1) {
        updateStatisticsTab();
    }
}

// Mettre à jour l'onglet des statistiques
void MainWindow::updateStatisticsTab()
{
    // Trouver le QTabWidget
    QTabWidget *tabWidget = findChild<QTabWidget*>();
    if (!tabWidget) {
        qDebug() << "⚠️ TabWidget non trouvé";
        return;
    }

    // Récupérer le widget de l'onglet Statistiques (index 1)
    QWidget *statsTab = tabWidget->widget(1);

    if (!statsTab) {
        qDebug() << "⚠️ Onglet Statistiques non trouvé";
        return;
    }

    // Supprimer l'ancien contenu s'il existe
    if (statsTab->layout()) {
        QLayoutItem *item;
        while ((item = statsTab->layout()->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->deleteLater();
            }
            delete item;
        }
        delete statsTab->layout();
    }

    // Créer le nouveau layout pour les statistiques
    QVBoxLayout *mainLayout = new QVBoxLayout(statsTab);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Titre de la section
    QLabel *titleLabel = new QLabel("📊 Tableau de Bord - Statistiques des Employés");
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2e86ab; padding: 10px; background: white; border-radius: 5px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Layout horizontal pour les cartes statistiques
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(15);

    // Récupérer les données pour les cartes
    QSqlQuery q;

    // Carte 1: Total Employés
    int totalEmployees = 0;
    q.exec("SELECT COUNT(*) FROM EMPLOYEE");
    if (q.next()) totalEmployees = q.value(0).toInt();
    QGroupBox *card1 = createStatCard("👥 Total Employés", QString::number(totalEmployees), "#4A90E2");
    cardsLayout->addWidget(card1);

    // Carte 2: Salaire Total
    double totalSalary = 0.0;
    q.exec("SELECT SUM(salaire) FROM EMPLOYEE");
    if (q.next()) totalSalary = q.value(0).toDouble();
    QGroupBox *card2 = createStatCard("💰 Salaire Total", QString::number(totalSalary, 'f', 2) + " DT", "#50C878");
    cardsLayout->addWidget(card2);

    // Carte 3: Présents
    int presents = 0;
    q.exec("SELECT COUNT(*) FROM EMPLOYEE WHERE LOWER(presence) LIKE '%présent%' OR LOWER(presence) LIKE '%present%'");
    if (q.next()) presents = q.value(0).toInt();
    QGroupBox *card3 = createStatCard("✅ Présents", QString::number(presents), "#FF6B6B");
    cardsLayout->addWidget(card3);

    // Carte 4: Salaire Moyen
    double avgSalary = totalEmployees > 0 ? totalSalary / totalEmployees : 0;
    QGroupBox *card4 = createStatCard("📈 Salaire Moyen", QString::number(avgSalary, 'f', 2) + " DT", "#FFD93D");
    cardsLayout->addWidget(card4);

    mainLayout->addLayout(cardsLayout);

    // Layout horizontal pour les graphiques
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(15);

    // Graphique 1: Répartition des présences (Pie Chart)
    QChartView *presenceChart = createPresenceChart();
    presenceChart->setMinimumHeight(400);
    presenceChart->setMinimumWidth(400);
    chartsLayout->addWidget(presenceChart, 1);

    mainLayout->addLayout(chartsLayout);

    // Ajouter un espacement flexible en bas
    mainLayout->addStretch();

    statsTab->setLayout(mainLayout);
}

void MainWindow::createStatisticsWidget()
{
    // Cette fonction n'est plus utilisée
}

// Créer une carte statistique
QGroupBox* MainWindow::createStatCard(const QString &title, const QString &value, const QString &color)
{
    QGroupBox *card = new QGroupBox();
    card->setFixedHeight(120);
    card->setStyleSheet(QString(
                            "QGroupBox {"
                            "   background-color: white;"
                            "   border-left: 5px solid %1;"
                            "   border-radius: 8px;"
                            "   padding: 15px;"
                            "   margin: 5px;"
                            "}"
                            ).arg(color));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(10);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 13px; color: #666; font-weight: bold;");
    titleLabel->setWordWrap(true);

    QLabel *valueLabel = new QLabel(value);
    QFont valueFont;
    valueFont.setPointSize(22);
    valueFont.setBold(true);
    valueLabel->setFont(valueFont);
    valueLabel->setStyleSheet(QString("color: %1;").arg(color));
    valueLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);
    layout->addStretch();
    card->setLayout(layout);

    return card;
}

// Créer le graphique de présence (Pie Chart)
QChartView* MainWindow::createPresenceChart()
{
    QSqlQuery q("SELECT presence, COUNT(*) as count FROM EMPLOYEE WHERE presence IS NOT NULL GROUP BY presence");

    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);

    QMap<QString, int> presenceMap;
    int total = 0;

    while (q.next()) {
        QString presence = q.value(0).toString().toLower();
        int count = q.value(1).toInt();

        if (presence.contains("présent") || presence.contains("present")) {
            presenceMap["Présent"] += count;
        } else if (presence.contains("absent")) {
            presenceMap["Absent"] += count;
        } else {
            presenceMap["Non défini"] += count;
        }
        total += count;
    }

    // Si aucune donnée
    if (total == 0) {
        presenceMap["Aucune donnée"] = 1;
        total = 1;
    }

    for (auto it = presenceMap.begin(); it != presenceMap.end(); ++it) {
        double percentage = total > 0 ? (it.value() * 100.0) / total : 0;
        QPieSlice *slice = series->append(it.key() + QString(" (%1%)").arg(percentage, 0, 'f', 1), it.value());

        if (it.key() == "Présent") {
            slice->setColor(QColor("#50C878"));
            slice->setExploded(true);
            slice->setExplodeDistanceFactor(0.05);
        } else if (it.key() == "Absent") {
            slice->setColor(QColor("#FF6B6B"));
        } else {
            slice->setColor(QColor("#FFD93D"));
        }

        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);
        slice->setLabelFont(QFont("Arial", 9, QFont::Bold));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("📊 Répartition des Présences");
    chart->setTitleFont(QFont("Arial", 13, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setFont(QFont("Arial", 9));
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setMargins(QMargins(10, 10, 10, 10));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: white; border-radius: 10px; border: 1px solid #e0e0e0;");

    return chartView;
}

// Créer le graphique des salaires par métier (Bar Chart)
QChartView* MainWindow::createSalaryByJobChart()
{
    QSqlQuery q("SELECT metier, SUM(salaire) as total_salaire, COUNT(*) as nb_employes FROM EMPLOYEE WHERE metier IS NOT NULL AND metier != '' GROUP BY metier ORDER BY total_salaire DESC LIMIT 8");

    QBarSet *salarySet = new QBarSet("Salaire Total (DT)");
    salarySet->setColor(QColor("#4A90E2"));

    QStringList categories;
    double maxSalary = 0;

    while (q.next()) {
        QString metier = q.value(0).toString();
        if (metier.isEmpty()) metier = "Non défini";
        // Raccourcir les noms trop longs
        if (metier.length() > 12) metier = metier.left(10) + "..";

        double totalSalaire = q.value(1).toDouble();
        if (totalSalaire > maxSalary) maxSalary = totalSalaire;

        categories << metier;
        *salarySet << totalSalaire;
    }

    // Si aucune donnée, ajouter des données factices
    if (categories.isEmpty()) {
        categories << "Aucune donnée";
        *salarySet << 0;
    }

    QBarSeries *series = new QBarSeries();
    series->append(salarySet);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("💼 Salaire Total par Métier (Top 8)");
    chart->setTitleFont(QFont("Arial", 13, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(Qt::white));

    // Marges pour éviter le découpage
    chart->setMargins(QMargins(10, 10, 10, 10));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(-35);
    axisX->setLabelsFont(QFont("Arial", 8));
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Salaire (DT)");
    axisY->setTitleFont(QFont("Arial", 9, QFont::Bold));
    axisY->setLabelFormat("%.0f");
    axisY->setLabelsFont(QFont("Arial", 8));
    // Ajuster l'échelle pour laisser de l'espace
    axisY->setRange(0, maxSalary * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);
    chart->legend()->setFont(QFont("Arial", 9));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background: white; border-radius: 10px; border: 1px solid #e0e0e0;");

    return chartView;
}

void MainWindow::updateStatistics()
{
    // Cette fonction peut être appelée mais ne fait rien
    // Les stats se mettent à jour via updateStatisticsTab()
}

static bool isDigitsOnly(const QString &s)
{
    for (const QChar &ch : s)
        if (!ch.isDigit()) return false;
    return true;
}

static bool isValidDecimal(const QString &s)
{
    bool ok;
    s.toDouble(&ok);
    return ok;
}

void MainWindow::refreshEmployeTable()
{
    onDisplayAllEmployes();
}

void MainWindow::onAddEmploye()
{
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
    refreshEmployeTable();
    autoRefreshStatistics(); // 🆕 Mise à jour automatique des stats
    QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
}

void MainWindow::onDeleteEmploye()
{
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

    refreshEmployeTable();
    clearFields();
    autoRefreshStatistics(); // 🆕 Mise à jour automatique des stats
    QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
}

void MainWindow::onUpdateEmploye()
{
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

    refreshEmployeTable();
    clearFields();
    autoRefreshStatistics(); // 🆕 Mise à jour automatique des stats
    QMessageBox::information(this, "Succès", "Employé mis à jour avec succès.");
}

void MainWindow::onTableRowSelected(int row, int)
{
    if (row < 0) return;

    if (QTableWidgetItem *cinItem = ui->tableWidget->item(row, 0)) ui->cin_2->setText(cinItem->text());
    if (QTableWidgetItem *nomItem = ui->tableWidget->item(row, 1)) ui->nom_2->setText(nomItem->text());
    if (QTableWidgetItem *prenomItem = ui->tableWidget->item(row, 2)) ui->prenom_2->setText(prenomItem->text());
    if (QTableWidgetItem *telephoneItem = ui->tableWidget->item(row, 3)) ui->telephone_2->setText(telephoneItem->text());
    if (QTableWidgetItem *adresseItem = ui->tableWidget->item(row, 4)) ui->adresse_2->setText(adresseItem->text());
    if (QTableWidgetItem *metierItem = ui->tableWidget->item(row, 5)) ui->metier_2->setText(metierItem->text());
    if (QTableWidgetItem *presenceItem = ui->tableWidget->item(row, 6)) ui->presence_2->setText(presenceItem->text());
    if (QTableWidgetItem *salaireItem = ui->tableWidget->item(row, 7)) ui->salaire_2->setText(salaireItem->text());
}

void MainWindow::clearFields()
{
    ui->cin_2->clear();
    ui->nom_2->clear();
    ui->prenom_2->clear();
    ui->adresse_2->clear();
    ui->telephone_2->clear();
    ui->metier_2->clear();
    ui->presence_2->clear();
    ui->salaire_2->clear();
    ui->cin_2->setFocus();
}

void MainWindow::onDisplayEmploye()
{
    const QString cin = ui->cin_2->text().trimmed();
    if (cin.isEmpty()) return;

    Employee emp = Employee::fetchByCin(cin);
    if (emp.getCin().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Employé non trouvé.");
        return;
    }

    ui->tableWidget->setRowCount(0);
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

void MainWindow::onDisplayAllEmployes()
{
    QVector<Employee> employes = Employee::fetchAll();
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
    statusBar()->showMessage(QString("Nombre total d'employés : %1").arg(employes.size()));
}

void MainWindow::onSearchEmploye()
{
    const QString search = ui->cin_4->text().trimmed();
    if (search.isEmpty()) {
        onDisplayAllEmployes();
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
    statusBar()->showMessage(QString("Résultats affichés : %1").arg(ui->tableWidget->rowCount()));
}

void MainWindow::onTrierParSalaire()
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
    statusBar()->showMessage(QString("Employés triés par salaire (décroissant) : %1").arg(employes.size()));
}
void MainWindow::exportPdf()
{
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

    int margin = 30;
    int rowHeight = 25;
    int columnWidth = 60;
    int logoWidth = 70;
    int logoHeight = 70;
    int logoY = margin;
    int currentY ;
    currentY = qMax(currentY, logoY + logoHeight + 10);


    QRect pageRect = printer.pageLayout().paintRectPixels(printer.resolution());


    // Charger le logo
    QPixmap logo("C:/Users/amine/OneDrive/Images/logo4.png");

    QPixmap scaledLogo = logo.scaled(logoWidth, logoHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int logoX = pageRect.width() - margin - logoWidth;
    painter.drawPixmap(logoX, logoY, scaledLogo);

    // * titre *
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(QRect(margin, currentY, pageRect.width() - 2 * margin, 30), "Tableau des Employées : ");
    currentY += 40;

    // * couleurs et styles*
    QPen borderPen(Qt::black);
    borderPen.setWidth(2);
    QBrush headerBrush(QColor(200, 200, 250)); //  bleu clair : les en-têtes
    QBrush cellBrush(QColor(250, 250, 250));   //  gris clair : les lignes
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setBrush(headerBrush);
    painter.setPen(borderPen);

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
        QString header = ui->tableWidget->horizontalHeaderItem(col)->text();
        QRect cellRect(margin + col * columnWidth, currentY, columnWidth, rowHeight);
        painter.drawRect(cellRect);
        painter.drawText(cellRect, Qt::AlignCenter, header);
    }
    currentY += rowHeight;

    // *Dessiner les données avec alternance de couleurs*
    painter.setFont(QFont("Arial", 10));

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        painter.setBrush(row % 2 == 0 ? cellBrush : Qt::white); // Alternance des couleurs

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            QRect cellRect(margin + col * columnWidth, currentY, columnWidth, rowHeight);

            // Dessiner la cellule avec une bordure
            painter.drawRect(cellRect);

            if (item) {
                painter.drawText(cellRect, Qt::AlignCenter, item->text());
            }
        }
        currentY += rowHeight;

        // *Gestion de la pagination*
        if (currentY > pageRect.height() - margin) {
            printer.newPage();
            currentY = margin + 40; // Reprendre après le titre
        }
    }

    QString exportDateTime = "Crée le : " + QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    painter.setFont(QFont("Arial", 10, QFont::Normal));

    // Si l'on dépasse la page, créer une nouvelle page
    if (currentY + 30 > pageRect.height() - margin) {
        printer.newPage();
        currentY = margin;
    }

    currentY += 20;
    painter.drawText(margin, currentY, exportDateTime);


    painter.end();
    QMessageBox::information(this, "Succès", "Le tableau a été exporté en PDF avec succès.");
}

// ============ GESTION RFID ============

void MainWindow::onRfidScanned(const QString &uid)
{
    // Afficher le dialog avec l'UID
    m_rfidDialog->showRfidUid(uid);
    
    // Afficher aussi dans la barre de statut
    statusBar()->showMessage(QString("💳 Carte scannée: %1").arg(uid), 3000);
}

// ============ INITIALISER RFID (après DB connectée) ============

void MainWindow::initializeRFID()
{
    qDebug() << "🔧 Initializing RFID system...";
    
    // ============ INITIALISATION RFID ============
    m_rfidController = new RFIDController(this);
    m_arduinoManager = new ArduinoManager(this);
    m_rfidDialog = new RfidDisplayDialog(this);

    // Quand une carte RFID est scannée
    connect(m_arduinoManager, &ArduinoManager::rfidScanned, 
            m_rfidController, &RFIDController::procesRfidCard);
    
    // ✅ CARTE RECONNUE → Accès automatique
    connect(m_rfidController, &RFIDController::rfidProcessed,
            this, [this](const QString &uid, const QString &message) {
        QMessageBox::information(this, "✅ Accès Autorisé", message);
        statusBar()->showMessage(QString("✅ Carte acceptée: %1").arg(uid), 3000);
        // Fermer pour montrer le login avec accès automatique
        this->close();
    });
    
    // ❌ CARTE NON RECONNUE → Login normal
    connect(m_rfidController, &RFIDController::rfidNotRecognized,
            this, [this](const QString &uid) {
        QMessageBox::information(this, "🔐 Connexion Requise", 
            QString("Carte '%1' non autorisée.\nVeuillez vous connecter avec vos identifiants.").arg(uid));
        statusBar()->showMessage("Connexion normale requise", 5000);
        // Fermer pour montrer le login normal
        this->close();
    });
    
    connect(m_rfidController, &RFIDController::rfidError,
            this, [this](const QString &error) {
        QMessageBox::warning(this, "❌ Erreur RFID", error);
    });

    // Ouvrir le port COM10 (changez si nécessaire)
    QString arduinoPort = "COM10";  // ⚠️ MODIFIER ICI LE PORT
    if (m_arduinoManager->open(arduinoPort, 9600)) {
        statusBar()->showMessage(QString("✅ Arduino connecté sur %1 - Prêt pour scan RFID").arg(arduinoPort), 5000);
        qDebug() << "✅ Arduino RFID prêt sur" << arduinoPort;
    } else {
        statusBar()->showMessage("❌ Arduino non connecté - Vérifiez le port COM", 10000);
        qDebug() << "❌ Échec connexion Arduino sur" << arduinoPort;
        
        // Afficher un message explicatif
        QMessageBox::information(this, "Info RFID", 
            QString("Arduino non détecté sur %1.\n\n"
                    "Vérifiez que l'Arduino est branché et que le port est correct.\n"
                    "Redémarrez l'application après branchement.").arg(arduinoPort));
    }
    
    // Ajouter un bouton de test RFID dans la barre de statut
    QPushButton *testRfidBtn = new QPushButton("🧪 Test RFID", this);
    testRfidBtn->setStyleSheet("QPushButton { background-color: #4A90E2; color: white; border-radius: 5px; padding: 5px 15px; font-weight: bold; }"
                                "QPushButton:hover { background-color: #357ABD; }");
    connect(testRfidBtn, &QPushButton::clicked, this, [this]() {
        m_rfidController->procesRfidCard("TEST123456ABCD");
        statusBar()->showMessage("🧪 Test RFID simulé", 3000);
    });
    statusBar()->addPermanentWidget(testRfidBtn);
    
    qDebug() << "✅ RFID system initialized";
}
