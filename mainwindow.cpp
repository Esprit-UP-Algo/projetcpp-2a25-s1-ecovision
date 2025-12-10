#include "mainwindow.h"
#include <QApplication>
#include <QStackedWidget>
#include <QIcon>
#include <QSize>
#include <QCursor>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QLineEdit>
#include <QStyleFactory>
#include <QTextEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QDebug>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSeries>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "resident.h"
#include <QDateEdit>
#include "piechartwidget.h"
#include <QDate>
#include <QFontDatabase>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>

// ====================================================================
// CONSTRUCTOR / DESTRUCTOR
// ====================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , sidebar(nullptr)
    , mainStack(nullptr)
    , addMemberBtn(nullptr)
    , residentsTableWidget(nullptr)
    , statsWidget(nullptr)
    , employmentChartWidget(nullptr)
    , searchBar(nullptr)
    , filterComboBox(nullptr)
    , networkManager(nullptr)
{
    setupUI();
    setupEmailConfig();
    refreshResidentList();
}

MainWindow::~MainWindow() {}

// ====================================================================
// EMAIL CONFIGURATION - USING RESEND.COM
// ====================================================================

void MainWindow::setupEmailConfig()
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onEmailSent);
    qDebug() << "📧 Resend.com email configuration initialized";
}

void MainWindow::sendWelcomeEmail(const QString& email, const QString& firstName, const QString& lastName)
{
    // For testing: Only send to your own email address
    QString testEmail = "manazizdaagi@gmail.com"; // ← YOUR EMAIL
    QString actualEmail = email;

    // Use test email instead of the resident's email for now
    QString recipientEmail = testEmail;

    if (email.isEmpty() || !email.contains('@')) {
        qDebug() << "❌ Invalid email address:" << email;
        return;
    }

    if (!networkManager) {
        qDebug() << "❌ Network manager not available";
        return;
    }

    // Resend.com API configuration
    QString apiKey = "re_QUDTdfP1_JvNTuUNczHCcZraXQA58qXDQ";
    QString fromEmail = "Eco Vision <onboarding@resend.dev>";
    qDebug() << "=== EMAIL DEBUG INFO ===";
    qDebug() << "📧 Recipient:" << email;
    qDebug() << "👤 First Name:" << firstName;
    qDebug() << "👤 Last Name:" << lastName;
    qDebug() << "🔑 Using Resend.com API";

    QUrl url("https://api.resend.com/emails");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    QJsonObject jsonData;
    jsonData["from"] = fromEmail;
    jsonData["to"] = email;
    jsonData["subject"] = "Bienvenue dans notre communauté Eco Vision!";

    // Email content in both HTML and plain text
    QString htmlContent = QString(
                              "<!DOCTYPE html>"
                              "<html>"
                              "<head>"
                              "    <meta charset='utf-8'>"
                              "    <style>"
                              "        body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; }"
                              "        .container { max-width: 600px; margin: 0 auto; padding: 20px; }"
                              "        .header { background: #5cb5f2; color: white; padding: 20px; text-align: center; border-radius: 8px 8px 0 0; }"
                              "        .content { background: #f9fafb; padding: 20px; border-radius: 0 0 8px 8px; }"
                              "        .welcome { font-size: 18px; font-weight: bold; color: #5cb5f2; }"
                              "    </style>"
                              "</head>"
                              "<body>"
                              "    <div class='container'>"
                              "        <div class='header'>"
                              "            <h1>🌿 Eco Vision</h1>"
                              "        </div>"
                              "        <div class='content'>"
                              "            <p class='welcome'>Bonjour %1 %2,</p>"
                              "            <p>Bienvenue dans notre communauté Eco Vision ! Nous sommes ravis de vous compter parmi nous.</p>"
                              "            <p>Votre inscription a été enregistrée avec succès et nous vous remercions de nous avoir rejoint.</p>"
                              "            <p>N'hésitez pas à nous contacter si vous avez des questions.</p>"
                              "            <br>"
                              "            <p>Cordialement,<br>"
                              "            <strong>L'équipe Eco Vision</strong></p>"
                              "        </div>"
                              "    </div>"
                              "</body>"
                              "</html>"
                              ).arg(firstName).arg(lastName);

    QString textContent = QString(
                              "Bonjour %1 %2,\n\n"
                              "Bienvenue dans notre communauté Eco Vision ! Nous sommes ravis de vous compter parmi nous.\n\n"
                              "Votre inscription a été enregistrée avec succès et nous vous remercions de nous avoir rejoint.\n\n"
                              "N'hésitez pas à nous contacter si vous avez des questions.\n\n"
                              "Cordialement,\n"
                              "L'équipe Eco Vision"
                              ).arg(firstName).arg(lastName);

    jsonData["html"] = htmlContent;
    jsonData["text"] = textContent;

    QJsonDocument doc(jsonData);
    QByteArray data = doc.toJson();

    qDebug() << "📤 Sending request to Resend.com...";
    qDebug() << "📦 JSON Data:" << QString(data);

    networkManager->post(request, data);
    qDebug() << "✅ Request sent to Resend.com API";
}

void MainWindow::onEmailSent(QNetworkReply* reply)
{
    QByteArray response = reply->readAll();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "=== EMAIL RESPONSE ===";
    qDebug() << "📡 HTTP Status:" << statusCode;
    qDebug() << "📄 Response:" << response;

    if (reply->error() == QNetworkReply::NoError && statusCode == 200) {
        qDebug() << "✅ TEST email sent successfully!";
        qDebug() << "📧 Check your inbox: manazizdaagi@gmail.com";
        QMessageBox::information(this, "Test Email Envoyé",
                                 "Email de test envoyé avec succès!\n\n"
                                 "Vérifiez votre boîte de réception: manazizdaagi@gmail.com\n\n"
                                 "En mode production, l'email serait envoyé au résident.");
    } else {
        qDebug() << "❌ Failed to send test email";
        QMessageBox::warning(this, "Erreur Test Email",
                             "Impossible d'envoyer l'email de test.\n\n"
                             "Vérifiez votre clé API Resend.com");
    }
    reply->deleteLater();
}
// ====================================================================
// CORE SETUP
// ====================================================================

void MainWindow::setupUI()
{
    // --- Window Setup ---
    this->setFixedSize(1200, 800);

    // ---------------- Load Inter font ----------------
    int fontId = QFontDatabase::addApplicationFont(":/fonts/Inter-Regular.ttf");
    QString interFamily = (fontId != -1 && !QFontDatabase::applicationFontFamilies(fontId).isEmpty())
                              ? QFontDatabase::applicationFontFamilies(fontId).at(0)
                              : "Segoe UI";

    QFont appFont(interFamily, 11);
    qApp->setFont(appFont);

    // ---------------- Central widget ----------------
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // ---------------- Sidebar ----------------
    sidebar = new QFrame(centralWidget);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet("background-color: #5cb5f2;");

    QVBoxLayout* sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(20,20,20,20);
    sidebarLayout->setSpacing(15);

    QLabel* appTitle = new QLabel("Eco Vision");
    appTitle->setStyleSheet("color: white; font-size: 30px; font-weight: bold;");
    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addSpacing(30);

    // Buttons (Note: Only Gestion Résidents is currently connected)
    sidebarLayout->addWidget(createSidebarButton("Gestion Employés"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Locaux"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Ressources"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Réclamations"));

    QPushButton* residentsBtn = createSidebarButton("Gestion Résidents");
    // Assuming you want the residents list to be the default view
    connect(residentsBtn, &QPushButton::clicked, this, &MainWindow::showResidentsList);
    sidebarLayout->addWidget(residentsBtn);

    sidebarLayout->addStretch();

    // ---------------- Main Stacked Widget ----------------
    mainStack = new QStackedWidget(centralWidget);

    QWidget* residentsListWidget = createResidentsListWidget();
    QWidget* addMemberWidget = createAddMemberWidget();

    mainStack->addWidget(residentsListWidget); // Index 0
    mainStack->addWidget(addMemberWidget);     // Index 1

    // ---------------- Add to main layout ----------------
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainStack);
}

// ====================================================================
// DATA REFRESH FUNCTION (Reads from DB and populates table)
// ====================================================================

void MainWindow::refreshResidentList()
{
    if (!residentsTableWidget) {
        return;
    }

    QList<Resident> residentList = Resident::afficherTous();

    residentsTableWidget->setRowCount(0); // Clear existing content
    residentsTableWidget->setRowCount(residentList.size());

    // UPDATE: Changed table columns to match new Resident structure
    residentsTableWidget->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Téléphone", "Statut Emploi"});

    // Populate the table
    for (int i = 0; i < residentList.size(); ++i) {
        const Resident& r = residentList.at(i);

        // UPDATE: Use CIN instead of ID
        QTableWidgetItem* cinItem = new QTableWidgetItem(r.getCin());
        residentsTableWidget->setItem(i, 0, cinItem);

        QTableWidgetItem* nomItem = new QTableWidgetItem(r.getNom());
        residentsTableWidget->setItem(i, 1, nomItem);

        QTableWidgetItem* prenomItem = new QTableWidgetItem(r.getPrenom());
        residentsTableWidget->setItem(i, 2, prenomItem);

        QTableWidgetItem* dnItem = new QTableWidgetItem(r.getDateNaissance().toString("dd/MM/yyyy"));
        residentsTableWidget->setItem(i, 3, dnItem);

        QTableWidgetItem* telItem = new QTableWidgetItem(r.getTelephone());
        residentsTableWidget->setItem(i, 4, telItem);

        // UPDATE: Add job status column
        QTableWidgetItem* jobItem = new QTableWidgetItem(r.getStatutEmploi());
        residentsTableWidget->setItem(i, 5, jobItem);

        // Align all items
        for(int j = 0; j < residentsTableWidget->columnCount(); ++j) {
            if (residentsTableWidget->item(i, j)) {
                residentsTableWidget->item(i, j)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }
    }

    // Refresh the charts after updating the resident list
    refreshCharts();
}

// ====================================================================
// SEARCH FUNCTIONALITY
// ====================================================================

void MainWindow::performSearch()
{
    if (!residentsTableWidget || !searchBar) return;

    QString searchText = searchBar->text().trimmed().toLower();
    QString filterType = filterComboBox->currentText();

    if (searchText.isEmpty()) {
        // If search is empty, show all residents
        refreshResidentList();
        return;
    }

    QList<Resident> allResidents = Resident::afficherTous();
    QList<Resident> filteredResidents;

    for (const Resident& resident : allResidents) {
        bool matches = false;

        if (filterType == "Nom") {
            QString fullName = resident.getNom().toLower() + " " + resident.getPrenom().toLower();
            matches = resident.getNom().toLower().contains(searchText) ||
                      resident.getPrenom().toLower().contains(searchText) ||
                      fullName.contains(searchText);
        }
        else if (filterType == "Âge") {
            // Calculate age from birth date
            QDate currentDate = QDate::currentDate();
            QDate birthDate = resident.getDateNaissance();
            int age = birthDate.daysTo(currentDate) / 365;

            matches = QString::number(age).contains(searchText);
        }
        else if (filterType == "CIN") {
            matches = resident.getCin().toLower().contains(searchText);
        }

        if (matches) {
            filteredResidents.append(resident);
        }
    }

    // Update table with filtered results
    residentsTableWidget->setRowCount(0);
    residentsTableWidget->setRowCount(filteredResidents.size());

    for (int i = 0; i < filteredResidents.size(); ++i) {
        const Resident& r = filteredResidents.at(i);

        residentsTableWidget->setItem(i, 0, new QTableWidgetItem(r.getCin()));
        residentsTableWidget->setItem(i, 1, new QTableWidgetItem(r.getNom()));
        residentsTableWidget->setItem(i, 2, new QTableWidgetItem(r.getPrenom()));
        residentsTableWidget->setItem(i, 3, new QTableWidgetItem(r.getDateNaissance().toString("dd/MM/yyyy")));
        residentsTableWidget->setItem(i, 4, new QTableWidgetItem(r.getTelephone()));
        residentsTableWidget->setItem(i, 5, new QTableWidgetItem(r.getStatutEmploi()));

        // Align all items
        for(int j = 0; j < residentsTableWidget->columnCount(); ++j) {
            if (residentsTableWidget->item(i, j)) {
                residentsTableWidget->item(i, j)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }
    }
}

// ====================================================================
// CHART REFRESH FUNCTION
// ====================================================================

void MainWindow::refreshCharts()
{
    // Refresh employment chart
    if (employmentChartWidget && employmentChartWidget->layout() && employmentChartWidget->layout()->count() > 0) {
        QLayoutItem* item = employmentChartWidget->layout()->takeAt(0);
        if (item && item->widget()) {
            delete item->widget();
        }
        delete item;

        QWidget* newEmploymentChart = createEmploymentChartContent();
        employmentChartWidget->layout()->addWidget(newEmploymentChart);
    }

    // Refresh stats widget (age distribution)
    if (statsWidget && statsWidget->layout() && statsWidget->layout()->count() > 1) {
        // Remove the chart widget (index 1, after title at index 0)
        QLayoutItem* item = statsWidget->layout()->takeAt(1);
        if (item && item->widget()) {
            delete item->widget();
        }
        delete item;

        QWidget* newStatsChart = createAgeDistributionChart();
        statsWidget->layout()->addWidget(newStatsChart);
    }
}

// ====================================================================
// HELPER FUNCTIONS IMPLEMENTATIONS (UI elements)
// ====================================================================

QPushButton* MainWindow::createSidebarButton(const QString &text, const QString &iconPath)
{
    QPushButton* btn = new QPushButton("  " + text);
    btn->setFixedHeight(40);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QString style = "QPushButton {"
                    "color: white;"
                    "background: transparent;"
                    "border: none;"
                    "font-size: 14px;"
                    "text-align: left;"
                    "padding: 8px 12px;"
                    "}"
                    "QPushButton:hover {"
                    "background-color: rgba(255,255,255,0.15);"
                    "border-radius: 8px;"
                    "}";

    btn->setStyleSheet(style);

    if (!iconPath.isEmpty()) {
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(18,18));
    }

    return btn;
}

QFrame* MainWindow::createHeaderFrame()
{
    QFrame* frame = new QFrame();
    QHBoxLayout* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QLabel* filterLabel = new QLabel("Filtrer par");
    filterLabel->setStyleSheet("color: #6B7280; font-size: 14px;");

    filterComboBox = new QComboBox();
    // CHANGED: Updated filter options to Nom, Âge, CIN
    filterComboBox->addItems({"Nom", "Âge", "CIN"});
    filterComboBox->setCurrentIndex(0);
    filterComboBox->setFixedSize(100, 30);
    filterComboBox->setEditable(false);

    filterComboBox->setStyleSheet(
        "QComboBox {"
        "  border: 1px solid #22252a;"
        "  border-radius: 6px;"
        "  background-color: white;"
        "  color: black;"
        "}"
        "QComboBox::drop-down {"
        "  border: none;"
        "}"
        "QComboBox::down-arrow {"
        "  image: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "  color: black;"
        "  background-color: white;"
        "  selection-background-color: #3498db;"
        "  selection-color: white;"
        "}"
        );

    searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Chercher dans les résidents");
    searchBar->setFixedHeight(35);
    searchBar->setStyleSheet("QLineEdit { padding: 5px 10px; border: 1px solid #E5E7EB; border-radius: 6px; background-color: white; color: #111827;}");

    // Connect search functionality
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::performSearch);

    layout->addStretch(1);
    layout->addWidget(filterLabel);
    layout->addWidget(filterComboBox);
    layout->addWidget(searchBar);

    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->setStretchFactor(searchBar, 2);
    layout->setStretchFactor(filterComboBox, 0);

    return frame;
}

// Stores the pointer and sets up table look
QTableWidget* MainWindow::createStaffTable(QWidget *parent)
{
    // UPDATE: Changed to 6 columns to include CIN and Job Status
    QTableWidget* table = new QTableWidget(0, 6, parent);

    // FIX 2: Store the pointer globally
    residentsTableWidget = table;

    // UPDATE: Updated column headers
    table->setHorizontalHeaderLabels({"CIN", "Nom", "Prénom", "Date de naissance", "Téléphone", "Statut Emploi"});

    // --- Styling and Configuration ---
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);

    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: white; border-bottom: 1px solid #E5E7EB; font-weight: bold; padding: 10px 0; color: #6B7280; }");

    table->setShowGrid(false);
    table->setFrameShape(QFrame::NoFrame);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    table->setStyleSheet(
        "QTableWidget {"
        "    padding: 0 10px;"
        "    background-color: white;"
        "    color: #2D3748;"
        "}"
        "QTableWidget::item {"
        "    padding: 10px 0;"
        "    border-bottom: 1px solid #F3F4F6;"
        "    color: #2D3748;"
        "}"
        "QTableWidget::item:hover {"
        "    background-color: #F3F4F6;"
        "    cursor: pointer;"
        "    color: #2D3748;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}"
        );

    // Connect the item click signal
    connect(table, &QTableWidget::itemClicked, this, &MainWindow::onTableItemClicked);

    // --- Column Widths and Sizing ---
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed); // CIN
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Nom
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Prénom
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch); // Date naissance
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Téléphone
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents); // Statut Emploi
    table->setColumnWidth(0, 100); // Wider for CIN

    table->verticalScrollBar()->setStyleSheet("QScrollBar { width: 0px; }");

    return table;
}

QFrame* MainWindow::createPaginationFrame()
{
    QFrame* frame = new QFrame();
    frame->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(5);

    layout->addStretch();

    QStringList pages = {"<", "1", "2", "3", "...", "9", ">"};
    for (const QString& page : pages) {
        QPushButton* btn = new QPushButton(page);
        btn->setFixedSize(30, 30);

        QString style = "QPushButton { border: none; border-radius: 6px; font-weight: 500; color: #111827;}"
                        "QPushButton:hover { background-color: #4ca0d4; color: white; }"
                        "QPushButton:pressed { background-color: #3d8bb7; }";

        if (page == "1") {
            style = "QPushButton { background-color: #4ca0d4; border: none; border-radius: 6px; font-weight: bold; }";
        }
        if (page == "...") {
            btn->setEnabled(false);
            style = "QPushButton { border: none; color: #9CA3AF; }";
        }

        btn->setStyleSheet(style);
        layout->addWidget(btn);
    }

    layout->addStretch();

    return frame;
}

// ====================================================================
// AGE DISTRIBUTION CHART
// ====================================================================

QWidget* MainWindow::createAgeDistributionChart()
{
    QWidget* chartContainer = new QWidget();
    chartContainer->setFixedHeight(200);
    chartContainer->setStyleSheet("background-color: #f8fafc; border-radius: 8px; border: 1px solid #e2e8f0;");

    QVBoxLayout* chartLayout = new QVBoxLayout(chartContainer);
    chartLayout->setContentsMargins(15, 10, 15, 10);

    // Get resident data and calculate age distribution
    QList<Resident> residents = Resident::afficherTous();

    // Define age groups
    QMap<QString, int> ageGroups = {
        {"0-18", 0},
        {"19-25", 0},
        {"26-35", 0},
        {"36-50", 0},
        {"51-65", 0},
        {"65+", 0}
    };

    // Calculate current date for age calculation
    QDate currentDate = QDate::currentDate();

    // Count residents in each age group
    for (const Resident& resident : residents) {
        QDate birthDate = resident.getDateNaissance();
        int age = birthDate.daysTo(currentDate) / 365; // Approximate age in years

        if (age <= 18) {
            ageGroups["0-18"]++;
        } else if (age <= 25) {
            ageGroups["19-25"]++;
        } else if (age <= 35) {
            ageGroups["26-35"]++;
        } else if (age <= 50) {
            ageGroups["36-50"]++;
        } else if (age <= 65) {
            ageGroups["51-65"]++;
        } else {
            ageGroups["65+"]++;
        }
    }

    // Create visual bar chart representation
    QWidget* barsContainer = new QWidget();
    QHBoxLayout* barsLayout = new QHBoxLayout(barsContainer);
    barsLayout->setContentsMargins(10, 20, 10, 10);
    barsLayout->setSpacing(8);
    barsLayout->setAlignment(Qt::AlignBottom);

    // Find maximum value for scaling
    int maxCount = 0;
    for (int count : ageGroups.values()) {
        if (count > maxCount) maxCount = count;
    }

    // Define colors for bars
    QStringList barColors = {"#5cb5f2", "#34d399", "#f59e0b", "#ef4444", "#8b5cf6", "#06b6d4"};

    int colorIndex = 0;
    for (const QString& ageGroup : ageGroups.keys()) {
        int count = ageGroups[ageGroup];
        double percentage = (maxCount > 0) ? (count * 80.0) / maxCount : 0; // Scale to max 80% height

        QWidget* barGroup = new QWidget();
        QVBoxLayout* barLayout = new QVBoxLayout(barGroup);
        barLayout->setContentsMargins(0, 0, 0, 0);
        barLayout->setSpacing(5);
        barLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

        // Bar
        QWidget* bar = new QWidget();
        bar->setFixedHeight(percentage);
        bar->setStyleSheet(QString("background-color: %1; border-radius: 4px 4px 0 0; min-height: 4px;").arg(barColors[colorIndex % barColors.size()]));

        // Count label
        QLabel* countLabel = new QLabel(QString::number(count));
        countLabel->setStyleSheet("font-size: 10px; font-weight: bold; color: #475569;");
        countLabel->setAlignment(Qt::AlignCenter);

        // Age group label
        QLabel* ageLabel = new QLabel(ageGroup);
        ageLabel->setStyleSheet("font-size: 9px; color: #64748b;");
        ageLabel->setAlignment(Qt::AlignCenter);

        barLayout->addWidget(countLabel);
        barLayout->addWidget(bar);
        barLayout->addWidget(ageLabel);

        barsLayout->addWidget(barGroup);
        colorIndex++;
    }

    chartLayout->addWidget(barsContainer);

    // Add total residents label
    QLabel* totalLabel = new QLabel(QString("Total résidents: %1").arg(residents.size()));
    totalLabel->setStyleSheet("font-size: 11px; color: #64748b; font-weight: 500; text-align: center;");
    chartLayout->addWidget(totalLabel);

    return chartContainer;
}

QWidget* MainWindow::createStatsWidget()
{
    QWidget* statsWidget = new QWidget();
    statsWidget->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");

    QVBoxLayout* layout = new QVBoxLayout(statsWidget);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);

    QLabel* title = new QLabel("📊 Répartition par Âge");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #111827;");
    layout->addWidget(title);

    // Store reference to stats widget for refresh
    this->statsWidget = statsWidget;

    // Add age distribution chart
    QWidget* ageChart = createAgeDistributionChart();
    layout->addWidget(ageChart);

    return statsWidget;
}

// ====================================================================
// EMPLOYMENT CHART CONTENT (Separated for refresh)
// ====================================================================

QWidget* MainWindow::createEmploymentChartContent()
{
    QWidget* chartContent = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(chartContent);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    QLabel* title = new QLabel("📊 Statut d'Emploi");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #111827;");
    layout->addWidget(title);

    // Get resident data and calculate statistics
    QList<Resident> residents = Resident::afficherTous();

    // Count employment status
    QMap<QString, int> statusCount;
    int totalResidents = residents.size();

    for (const Resident& resident : residents) {
        QString status = resident.getStatutEmploi();
        if (!status.isEmpty() && status != "Statut d'emploi") {
            statusCount[status]++;
        }
    }

    // Create chart container
    QWidget* chartContainer = new QWidget();
    chartContainer->setFixedHeight(180);
    chartContainer->setStyleSheet("background-color: #f8fafc; border-radius: 8px; border: 1px solid #e2e8f0;");

    QHBoxLayout* chartLayout = new QHBoxLayout(chartContainer);
    chartLayout->setContentsMargins(20, 15, 20, 15);

    // Left side: Visual pie chart representation
    QWidget* pieContainer = new QWidget();
    pieContainer->setFixedSize(120, 120);

    QVBoxLayout* pieLayout = new QVBoxLayout(pieContainer);
    pieLayout->setContentsMargins(0, 0, 0, 0);
    pieLayout->setAlignment(Qt::AlignCenter);

    // Define colors for each status
    QMap<QString, QString> statusColors = {
        {"Employe", "#5cb5f2"},
        {"Chomeur", "#ef4444"},
        {"Etudiant", "#34d399"},
        {"Retraite", "#f59e0b"},
        {"Autre", "#8b5cf6"}
    };

    // Create and setup pie chart widget
    PieChartWidget* pieWidget = new PieChartWidget();
    pieWidget->setFixedSize(100, 100);

    // DEBUG: Print the data being sent to pie chart
    qDebug() << "Status count data:" << statusCount;
    qDebug() << "Status colors:" << statusColors;

    pieWidget->setData(statusCount, statusColors);

    pieLayout->addWidget(pieWidget);

    // Right side: Legend with percentages
    QWidget* legendWidget = new QWidget();
    QVBoxLayout* legendLayout = new QVBoxLayout(legendWidget);
    legendLayout->setSpacing(8);
    legendLayout->setContentsMargins(10, 5, 0, 5);

    // Create legend items
    bool hasData = false;
    for (const QString& status : statusCount.keys()) {
        if (status.isEmpty() || status == "Statut d'emploi") continue;

        int count = statusCount[status];
        if (count > 0) {
            double percentage = totalResidents > 0 ? (count * 100.0) / totalResidents : 0;

            QWidget* legendItem = new QWidget();
            QHBoxLayout* itemLayout = new QHBoxLayout(legendItem);
            itemLayout->setContentsMargins(0, 0, 0, 0);
            itemLayout->setSpacing(8);

            // Color indicator
            QLabel* colorLabel = new QLabel();
            colorLabel->setFixedSize(12, 12);
            colorLabel->setStyleSheet(QString("background-color: %1; border-radius: 6px; border: 1px solid #cbd5e1;").arg(statusColors.value(status, "#94a3b8")));

            // Status text and percentage
            QLabel* textLabel = new QLabel(QString("%1 (%2%) - %3").arg(status).arg(QString::number(percentage, 'f', 1)).arg(count));
            textLabel->setStyleSheet("font-size: 11px; color: #475569; font-weight: 500;");

            itemLayout->addWidget(colorLabel);
            itemLayout->addWidget(textLabel);
            itemLayout->addStretch();

            legendLayout->addWidget(legendItem);
            hasData = true;
        }
    }

    // If no data
    if (!hasData) {
        QLabel* noDataLabel = new QLabel("Aucune donnée disponible");
        noDataLabel->setStyleSheet("font-size: 12px; color: #94a3b8; font-style: italic;");
        legendLayout->addWidget(noDataLabel);
    }

    chartLayout->addWidget(pieContainer);
    chartLayout->addWidget(legendWidget);
    chartLayout->addStretch();

    layout->addWidget(chartContainer);

    return chartContent;
}

QWidget* MainWindow::createEmploymentChartWidget()
{
    QWidget* chartWidget = new QWidget();
    chartWidget->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");

    QVBoxLayout* layout = new QVBoxLayout(chartWidget);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);

    // Store reference to employment chart widget for refresh
    this->employmentChartWidget = chartWidget;

    // Add employment chart content
    QWidget* employmentChart = createEmploymentChartContent();
    layout->addWidget(employmentChart);

    return chartWidget;
}

QWidget* MainWindow::createResidentsListWidget()
{
    QWidget* residentsWidget = new QWidget();
    residentsWidget->setStyleSheet("background-color: #F9FAFB;");
    QVBoxLayout* contentLayout = new QVBoxLayout(residentsWidget);
    contentLayout->setContentsMargins(30,30,30,30);
    contentLayout->setSpacing(15);

    // 1. --- Title Row with Button ---
    QFrame* titleRow = new QFrame();
    titleRow->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(0);

    QLabel* title = new QLabel("Résidents");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #111827;");

    addMemberBtn = new QPushButton("+ Ajouter un membre");
    addMemberBtn->setFixedSize(150, 35);
    addMemberBtn->setCursor(Qt::PointingHandCursor);
    addMemberBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4ca0d4;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8bb7;"
        "}"
        );

    // Connect the button to show add member form
    connect(addMemberBtn, &QPushButton::clicked, this, &MainWindow::showAddMemberForm);

    titleLayout->addWidget(title);
    titleLayout->addStretch();
    titleLayout->addWidget(addMemberBtn);

    // 2. --- Header/Filter/Search Row (Top Bar) ---
    QFrame* headerFrame = createHeaderFrame();

    // 3. --- Main Table Container (White Background) ---
    QFrame* tableContainer = new QFrame();
    tableContainer->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableContainer);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    // 4. --- Table Widget (Inside Container) ---
    QTableWidget* staffTable = createStaffTable(tableContainer);
    tableLayout->addWidget(staffTable);

    // 5. --- Pagination Area ---
    QFrame* paginationFrame = createPaginationFrame();
    tableLayout->addWidget(paginationFrame);

    // 6. --- Stats and Map Widgets Container ---
    QFrame* widgetsContainer = new QFrame();
    widgetsContainer->setFrameShape(QFrame::NoFrame);
    QHBoxLayout* widgetsLayout = new QHBoxLayout(widgetsContainer);
    widgetsLayout->setContentsMargins(0, 10, 0, 0);
    widgetsLayout->setSpacing(15);

    QWidget* statsWidget = createStatsWidget();
    QWidget* employmentChartWidget = createEmploymentChartWidget();

    widgetsLayout->addWidget(statsWidget, 1);
    widgetsLayout->addWidget(employmentChartWidget, 1);

    // Add components to the main content layout
    contentLayout->addWidget(titleRow);
    contentLayout->addWidget(headerFrame);
    contentLayout->addWidget(tableContainer);
    contentLayout->addWidget(widgetsContainer);
    contentLayout->addStretch();

    return residentsWidget;
}

// ====================================================================
// WIDGET FOR ADDING RESIDENT (with DB INSERT logic and EMAIL)
// ====================================================================
QWidget* MainWindow::createAddMemberWidget()
{
    QWidget* addMemberWidget = new QWidget();
    addMemberWidget->setStyleSheet("background-color: #F9FAFB;");
    QVBoxLayout* mainLayout = new QVBoxLayout(addMemberWidget);
    mainLayout->setContentsMargins(30,30,30,30);
    mainLayout->setSpacing(20);

    // Header with back button
    QFrame* headerFrame = new QFrame();
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* backButton = new QPushButton("← Retour à la liste");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #5cb5f2;"
        "    border: 1px solid #5cb5f2;"
        "    border-radius: 6px;"
        "    padding: 8px 16px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}"
        );
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showResidentsList);

    QLabel* title = new QLabel("Ajouter un nouveau résident");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #111827;");

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    // Form content
    QFrame* formContainer = new QFrame();
    formContainer->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");
    formContainer->setMaximumWidth(800);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(40, 30, 40, 30);
    formLayout->setSpacing(25);

    QLabel* formTitle = new QLabel("Informations du résident");
    formTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-bottom: 10px;");
    formLayout->addWidget(formTitle);

    // ========== ADDED CIN FIELD ==========
    QLineEdit* cinField = new QLineEdit();
    cinField->setPlaceholderText("Numéro CIN");
    cinField->setMinimumHeight(45);
    // =====================================

    QLabel* personalInfoLabel = new QLabel("Informations personnelles");
    personalInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(personalInfoLabel);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);

    QLineEdit* firstNameField = new QLineEdit();
    firstNameField->setPlaceholderText("Prénom");
    firstNameField->setMinimumHeight(45);

    QLineEdit* lastNameField = new QLineEdit();
    lastNameField->setPlaceholderText("Nom");
    lastNameField->setMinimumHeight(45);

    // Apply common style for form fields
    QString fieldStyle = "QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}";
    cinField->setStyleSheet(fieldStyle);  // Added CIN field style
    firstNameField->setStyleSheet(fieldStyle);
    lastNameField->setStyleSheet(fieldStyle);

    // Add CIN field to form
    formLayout->addWidget(cinField);
    nameLayout->addWidget(firstNameField);
    nameLayout->addWidget(lastNameField);
    formLayout->addLayout(nameLayout);

    QLabel* contactInfoLabel = new QLabel("Informations de contact");
    contactInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(contactInfoLabel);

    QLineEdit* emailField = new QLineEdit();
    emailField->setPlaceholderText("Adresse email");
    emailField->setStyleSheet(fieldStyle);
    emailField->setMinimumHeight(45);
    formLayout->addWidget(emailField);

    QLineEdit* phoneField = new QLineEdit();
    phoneField->setPlaceholderText("Numéro de téléphone");
    phoneField->setStyleSheet(fieldStyle);
    phoneField->setMinimumHeight(45);
    formLayout->addWidget(phoneField);

    QLabel* addressInfoLabel = new QLabel("Adresse");
    addressInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(addressInfoLabel);

    QLineEdit* addressField = new QLineEdit();
    addressField->setPlaceholderText("Adresse complète");
    addressField->setStyleSheet(fieldStyle);
    addressField->setMinimumHeight(45);
    formLayout->addWidget(addressField);

    QHBoxLayout* dobGenderLayout = new QHBoxLayout();
    dobGenderLayout->setSpacing(15);

    // FIXED: Use QDateEdit instead of QLineEdit for date input
    QDateEdit* dobField = new QDateEdit();
    dobField->setDisplayFormat("dd/MM/yyyy");
    dobField->setDate(QDate::currentDate().addYears(-30)); // Default to 30 years ago
    dobField->setCalendarPopup(true);
    dobField->setMinimumHeight(45);

    QComboBox* genderCombo = new QComboBox();
    genderCombo->addItems({"Genre", "Homme", "Femme", "Autre"});

    // ========== ADDED JOB STATUS FIELD ==========
    QComboBox* jobCombo = new QComboBox();
    jobCombo->addItems({"Statut d'emploi", "Employé", "Chômeur", "Étudiant", "Retraité", "Autre"});
    // ============================================

    // ComboBox Style
    QString comboStyle =
        "QComboBox {"
        "    padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px;"
        "    background-color: white; color: #111827;"
        "    min-height: 20px;"  // FIXED: Changed from 45px to 20px
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 30px;"  // Increased width for better click area
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #6B7280;"
        "    width: 10px;"
        "    height: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #D1D5DB;"
        "    border-radius: 6px;"
        "    background-color: white;"
        "    selection-background-color: #5cb5f2;"
        "    color: #111827;"
        "    padding: 8px;"
        "    min-height: 40px;"  // Ensure dropdown items have enough height
        "}"
        "QComboBox QAbstractItemView::item {"
        "    min-height: 35px;"  // Ensure each item has enough height
        "    padding: 8px 12px;"  // Add padding to items
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}";

    // DateEdit Style
    QString dateStyle =
        "QDateEdit {"
        "    padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px;"
        "    background-color: white; color: #111827;"
        "}"
        "QDateEdit::drop-down {"
        "    border: none;"
        "    width: 30px;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #6B7280;"
        "    width: 10px;"
        "    height: 10px;"
        "}";

    dobField->setStyleSheet(dateStyle);
    genderCombo->setStyleSheet(comboStyle);
    jobCombo->setStyleSheet(comboStyle);  // Apply same style to job combo

    // FIXED: Set fixed height for combo boxes
    genderCombo->setFixedHeight(45);
    jobCombo->setFixedHeight(45);

    dobGenderLayout->addWidget(dobField);
    dobGenderLayout->addWidget(genderCombo);
    dobGenderLayout->addWidget(jobCombo);  // Add job combo to layout
    formLayout->addLayout(dobGenderLayout);

    // Form buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    QPushButton* cancelButton = new QPushButton("Annuler");
    cancelButton->setFixedSize(120, 45);
    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #6B7280;"
        "    border: 1px solid #D1D5DB;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F9FAFB;"
        "}"
        );

    QPushButton* submitButton = new QPushButton("Ajouter le résident");
    submitButton->setFixedSize(180, 45);
    submitButton->setCursor(Qt::PointingHandCursor);
    submitButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #4ca0d4;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8bb7;"
        "}"
        );

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(submitButton);

    formLayout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::showResidentsList);

    // UPDATED: Connect submit button with CIN and Job Status - FIXED validation + EMAIL
    connect(submitButton, &QPushButton::clicked, [this, cinField, firstNameField, lastNameField, emailField, phoneField, addressField, dobField, genderCombo, jobCombo](){

        // --- Input Validation and Data Extraction ---
        QString cin = cinField->text().trimmed();
        QString nom = lastNameField->text().trimmed();
        QString prenom = firstNameField->text().trimmed();
        QString email = emailField->text().trimmed();
        QString telephone = phoneField->text().trimmed();
        QString adresse = addressField->text().trimmed();
        QString sexe = (genderCombo->currentIndex() > 0) ? genderCombo->currentText() : "";
        QString statutEmploi = (jobCombo->currentIndex() > 0) ? jobCombo->currentText() : "";

        // FIXED: Use QDateEdit which provides a valid QDate directly
        QDate dateNaissance = dobField->date();

        // FIXED: Debug output to see what's happening
        qDebug() << "Form validation:";
        qDebug() << "CIN:" << cin;
        qDebug() << "Nom:" << nom;
        qDebug() << "Prenom:" << prenom;
        qDebug() << "Date:" << dateNaissance.toString("dd/MM/yyyy");
        qDebug() << "Gender index:" << genderCombo->currentIndex();
        qDebug() << "Job index:" << jobCombo->currentIndex();

        // UPDATED: Include CIN and job status in validation
        if (cin.isEmpty() || nom.isEmpty() || prenom.isEmpty() || !dateNaissance.isValid() ||
            genderCombo->currentIndex() == 0 || jobCombo->currentIndex() == 0) {
            QMessageBox::warning(this, "Erreur de saisie",
                                 "Veuillez remplir tous les champs obligatoires: CIN, Nom, Prénom, Date de naissance, Genre et Statut d'emploi.");
            return;
        }

        // UPDATED: Create resident with CIN and job status
        Resident r(cin, nom, prenom, sexe, dateNaissance, adresse, email, telephone, statutEmploi);

        if (r.ajouter()) {
            QMessageBox::information(this, "Succès", "Le résident a été ajouté avec succès!");

            // ========== SEND WELCOME EMAIL ==========
            if (!email.isEmpty() && email.contains('@')) {
                sendWelcomeEmail(email, prenom, nom);
                qDebug() << "Welcome email sent to:" << email;
            } else {
                qDebug() << "Invalid email, skipping welcome email:" << email;
            }
            // ========================================

            // Clear fields on success
            cinField->clear();
            firstNameField->clear();
            lastNameField->clear();
            emailField->clear();
            phoneField->clear();
            addressField->clear();
            dobField->setDate(QDate::currentDate().addYears(-30)); // Reset to default
            genderCombo->setCurrentIndex(0);
            jobCombo->setCurrentIndex(0);

            refreshResidentList();
            showResidentsList();
        } else {
            QMessageBox::critical(this, "Erreur", "L'ajout du résident a échoué. Vérifiez la connexion à la base de données et les logs.");
        }
    });

    // Center the form
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(formContainer);
    centerLayout->addStretch();

    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
    return addMemberWidget;
}

// ====================================================================
// WIDGET FOR RESIDENT DETAILS
// ====================================================================
QWidget* MainWindow::createResidentDetailsWidget(int residentId)
{
    QWidget* detailsWidget = new QWidget();
    detailsWidget->setStyleSheet("background-color: #F9FAFB;");
    QVBoxLayout* mainLayout = new QVBoxLayout(detailsWidget);
    mainLayout->setContentsMargins(30,30,30,30);
    mainLayout->setSpacing(20);

    // Header with back button
    QFrame* headerFrame = new QFrame();
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* backButton = new QPushButton("← Retour à la liste");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #5cb5f2;"
        "    border: 1px solid #5cb5f2;"
        "    border-radius: 6px;"
        "    padding: 8px 16px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}"
        );
    connect(backButton, &QPushButton::clicked, this, &MainWindow::showResidentsList);

    QLabel* title = new QLabel("Détails du résident");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #111827;");

    // Action buttons
    QPushButton* editButton = new QPushButton("✏️ Modifier");
    // Connect to edit function - UPDATED for CIN
    connect(editButton, &QPushButton::clicked, [this, residentId]() {
        // Convert the residentId (which is actually CIN) back to QString
        QString residentCin = QString::number(residentId);
        showEditResidentForm(residentCin);
    });

    QPushButton* deleteButton = new QPushButton("🗑️ Supprimer");
    // Connect to delete function - UPDATED for CIN
    connect(deleteButton, &QPushButton::clicked, [this, residentId]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation",
                                      QString("Êtes-vous sûr de vouloir supprimer le résident CIN: %1 ?").arg(residentId),
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Convert residentId to QString for CIN
            QString residentCin = QString::number(residentId);
            Resident r = Resident::trouverParCin(residentCin);
            if (r.supprimer()) {
                QMessageBox::information(this, "Succès", "Résident supprimé.");
                refreshResidentList();
                showResidentsList();
            } else {
                QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
            }
        }
    });

    // Style buttons
    editButton->setFixedSize(120, 35);
    editButton->setCursor(Qt::PointingHandCursor);
    editButton->setStyleSheet(
        "QPushButton { background-color: #f59e0b; color: white; border: none; border-radius: 6px; font-weight: 500; font-size: 13px;}"
        "QPushButton:hover { background-color: #d97706;}"
        );

    deleteButton->setFixedSize(120, 35);
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setStyleSheet(
        "QPushButton { background-color: #ef4444; color: white; border: none; border-radius: 6px; font-weight: 500; font-size: 13px;}"
        "QPushButton:hover { background-color: #dc2626;}"
        );

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    headerLayout->addWidget(editButton);
    headerLayout->addSpacing(10);
    headerLayout->addWidget(deleteButton);

    // Main content container
    QFrame* contentContainer = new QFrame();
    contentContainer->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");

    QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
    contentLayout->setContentsMargins(40, 30, 40, 30);
    contentLayout->setSpacing(25);

    // Fetch resident data using the new trouverParCin function
    // UPDATE: Use trouverParCin instead of trouverParId
    Resident r = Resident::trouverParCin(QString::number(residentId));

    // UPDATE: Show CIN instead of ID
    QLabel* residentIdLabel = new QLabel("Résident CIN: " + r.getCin());
    residentIdLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #5cb5f2; margin-bottom: 10px; min-height: 20px;");
    contentLayout->addWidget(residentIdLabel);

    QLabel* personalInfoLabel = new QLabel("Informations personnelles");
    personalInfoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-bottom: 20px;");
    contentLayout->addWidget(personalInfoLabel);

    // Create info grid
    QGridLayout* infoGrid = new QGridLayout();
    infoGrid->setSpacing(20);
    infoGrid->setColumnMinimumWidth(0, 150);
    infoGrid->setColumnStretch(1, 1);

    // UPDATE: Updated labels and values for new Resident structure
    QStringList labels = {"CIN:", "Nom complet:", "Date de naissance:", "Genre:", "Email:", "Téléphone:", "Adresse:", "Statut d'emploi:"};
    QStringList values = {
        r.getCin(),
        r.getNom() + " " + r.getPrenom(),
        r.getDateNaissance().toString("dd/MM/yyyy"),
        r.getSexe(),
        r.getAdresseMail(),
        r.getTelephone(),
        r.getAdresse(),
        r.getStatutEmploi() // UPDATE: Use getStatutEmploi() instead of getEtatCivil()
    };

    for (int i = 0; i < labels.size(); ++i) {
        QLabel* label = new QLabel(labels[i]);
        label->setStyleSheet("font-weight: 600; color: #374151; font-size: 14px; min-height: 20px;");

        QLabel* value = new QLabel(values[i]);
        value->setStyleSheet("color: #6B7280; font-size: 14px; padding: 5px 0; min-height: 20px");
        value->setWordWrap(true);

        infoGrid->addWidget(label, i, 0);
        infoGrid->addWidget(value, i, 1);
    }

    contentLayout->addLayout(infoGrid);

    // Notes Section
    QLabel* notesLabel = new QLabel("Notes supplémentaires");
    notesLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-top: 30px; margin-bottom: 15px;");
    contentLayout->addWidget(notesLabel);

    QLabel* notesContent = new QLabel("Aucune note supplémentaire pour le moment.");
    notesContent->setStyleSheet("color: #6B7280; font-size: 14px; padding: 15px; background-color: #F9FAFB; border-radius: 6px; border: 1px solid #E5E7EB;");
    notesContent->setWordWrap(true);
    contentLayout->addWidget(notesContent);

    mainLayout->addWidget(headerFrame);
    mainLayout->addWidget(contentContainer);
    mainLayout->addStretch();

    return detailsWidget;
}

// ====================================================================
// EDIT RESIDENT WIDGET (NEW FUNCTION)
// ====================================================================
QWidget* MainWindow::createEditResidentWidget(QString cin)
{
    QWidget* editWidget = new QWidget();
    editWidget->setStyleSheet("background-color: #F9FAFB;");
    QVBoxLayout* mainLayout = new QVBoxLayout(editWidget);
    mainLayout->setContentsMargins(30,30,30,30);
    mainLayout->setSpacing(20);

    // Fetch existing resident data
    Resident resident = Resident::trouverParCin(cin);

    // Header with back button
    QFrame* headerFrame = new QFrame();
    QHBoxLayout* headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton* backButton = new QPushButton("← Retour aux détails");
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #5cb5f2;"
        "    border: 1px solid #5cb5f2;"
        "    border-radius: 6px;"
        "    padding: 8px 16px;"
        "    font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}"
        );
    connect(backButton, &QPushButton::clicked, [this, cin]() {
        showResidentDetails(cin.toInt());
    });

    QLabel* title = new QLabel("Modifier le résident");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #111827;");

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    headerLayout->addWidget(title);
    headerLayout->addStretch();

    // Form content
    QFrame* formContainer = new QFrame();
    formContainer->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");
    formContainer->setMaximumWidth(800);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(40, 30, 40, 30);
    formLayout->setSpacing(25);

    QLabel* formTitle = new QLabel("Modifier les informations du résident");
    formTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-bottom: 10px;");
    formLayout->addWidget(formTitle);

    // CIN Field (read-only for editing)
    QLabel* cinLabel = new QLabel("CIN");
    cinLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(cinLabel);

    QLineEdit* cinField = new QLineEdit();
    cinField->setText(resident.getCin());
    cinField->setReadOnly(true); // CIN cannot be changed
    cinField->setMinimumHeight(45);
    cinField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0; background-color: #f3f4f6;}");
    formLayout->addWidget(cinField);

    // Personal Info
    QLabel* personalInfoLabel = new QLabel("Informations personnelles");
    personalInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(personalInfoLabel);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);

    QLineEdit* firstNameField = new QLineEdit();
    firstNameField->setPlaceholderText("Prénom");
    firstNameField->setText(resident.getPrenom());
    firstNameField->setMinimumHeight(45);

    QLineEdit* lastNameField = new QLineEdit();
    lastNameField->setPlaceholderText("Nom");
    lastNameField->setText(resident.getNom());
    lastNameField->setMinimumHeight(45);

    QString fieldStyle = "QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}";
    firstNameField->setStyleSheet(fieldStyle);
    lastNameField->setStyleSheet(fieldStyle);

    nameLayout->addWidget(firstNameField);
    nameLayout->addWidget(lastNameField);
    formLayout->addLayout(nameLayout);

    // Contact Info
    QLabel* contactInfoLabel = new QLabel("Informations de contact");
    contactInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(contactInfoLabel);

    QLineEdit* emailField = new QLineEdit();
    emailField->setPlaceholderText("Adresse email");
    emailField->setText(resident.getAdresseMail());
    emailField->setStyleSheet(fieldStyle);
    emailField->setMinimumHeight(45);
    formLayout->addWidget(emailField);

    QLineEdit* phoneField = new QLineEdit();
    phoneField->setPlaceholderText("Numéro de téléphone");
    phoneField->setText(resident.getTelephone());
    phoneField->setStyleSheet(fieldStyle);
    phoneField->setMinimumHeight(45);
    formLayout->addWidget(phoneField);

    // Address
    QLabel* addressInfoLabel = new QLabel("Adresse");
    addressInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(addressInfoLabel);

    QLineEdit* addressField = new QLineEdit();
    addressField->setPlaceholderText("Adresse complète");
    addressField->setText(resident.getAdresse());
    addressField->setStyleSheet(fieldStyle);
    addressField->setMinimumHeight(45);
    formLayout->addWidget(addressField);

    // Date of Birth, Gender, and Job Status
    QHBoxLayout* dobGenderLayout = new QHBoxLayout();
    dobGenderLayout->setSpacing(15);

    // FIXED: Use QDateEdit instead of QLineEdit for date input
    QDateEdit* dobField = new QDateEdit();
    dobField->setDisplayFormat("dd/MM/yyyy");
    dobField->setDate(resident.getDateNaissance());
    dobField->setCalendarPopup(true);
    dobField->setMinimumHeight(45);

    QComboBox* genderCombo = new QComboBox();
    genderCombo->addItems({"Genre", "Homme", "Femme", "Autre"});
    // Set current gender
    int genderIndex = genderCombo->findText(resident.getSexe());
    if (genderIndex >= 0) genderCombo->setCurrentIndex(genderIndex);

    QComboBox* jobCombo = new QComboBox();
    jobCombo->addItems({"Statut d'emploi", "Employé", "Chômeur", "Étudiant", "Retraité", "Autre"});
    // Set current job status
    int jobIndex = jobCombo->findText(resident.getStatutEmploi());
    if (jobIndex >= 0) jobCombo->setCurrentIndex(jobIndex);

    QString comboStyle =
        "QComboBox {"
        "    padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px;"
        "    background-color: white; color: #111827; min-height: 20px;"
        "}"
        "QComboBox::drop-down { border: none; width: 30px; }"
        "QComboBox::down-arrow { image: none; border-left: 5px solid transparent; border-right: 5px solid transparent; border-top: 5px solid #6B7280; }"
        "QComboBox QAbstractItemView { border: 1px solid #D1D5DB; border-radius: 6px; background-color: white; selection-background-color: #5cb5f2; color: #111827; }"
        "QComboBox QAbstractItemView::item { padding: 8px; min-height: 30px; }"
        "QComboBox QAbstractItemView::item:selected { background-color: #5cb5f2; color: white;}";

    QString dateStyle =
        "QDateEdit {"
        "    padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px;"
        "    background-color: white; color: #111827;"
        "}"
        "QDateEdit::drop-down {"
        "    border: none;"
        "    width: 30px;"
        "}"
        "QDateEdit::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #6B7280;"
        "    width: 10px;"
        "    height: 10px;"
        "}";

    dobField->setStyleSheet(dateStyle);
    genderCombo->setStyleSheet(comboStyle);
    jobCombo->setStyleSheet(comboStyle);
    genderCombo->setFixedHeight(45);
    jobCombo->setFixedHeight(45);

    dobGenderLayout->addWidget(dobField);
    dobGenderLayout->addWidget(genderCombo);
    dobGenderLayout->addWidget(jobCombo);
    formLayout->addLayout(dobGenderLayout);

    // Form buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);

    QPushButton* cancelButton = new QPushButton("Annuler");
    cancelButton->setFixedSize(120, 45);
    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #6B7280;"
        "    border: 1px solid #D1D5DB;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #F9FAFB;"
        "}"
        );

    QPushButton* updateButton = new QPushButton("Mettre à jour");
    updateButton->setFixedSize(180, 45);
    updateButton->setCursor(Qt::PointingHandCursor);
    updateButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #f59e0b;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d97706;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #b45309;"
        "}"
        );

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(updateButton);

    formLayout->addLayout(buttonLayout);

    connect(cancelButton, &QPushButton::clicked, [this, cin]() {
        showResidentDetails(cin.toInt());
    });

    // Update button connection
    connect(updateButton, &QPushButton::clicked, [this, cinField, firstNameField, lastNameField, emailField, phoneField, addressField, dobField, genderCombo, jobCombo, cin]() {
        QString nom = lastNameField->text().trimmed();
        QString prenom = firstNameField->text().trimmed();
        QString email = emailField->text().trimmed();
        QString telephone = phoneField->text().trimmed();
        QString adresse = addressField->text().trimmed();
        QString sexe = (genderCombo->currentIndex() > 0) ? genderCombo->currentText() : "";
        QString statutEmploi = (jobCombo->currentIndex() > 0) ? jobCombo->currentText() : "";

        // FIXED: Use QDateEdit which provides a valid QDate directly
        QDate dateNaissance = dobField->date();

        if (nom.isEmpty() || prenom.isEmpty() || !dateNaissance.isValid() ||
            genderCombo->currentIndex() == 0 || jobCombo->currentIndex() == 0) {
            QMessageBox::warning(this, "Erreur de saisie",
                                 "Veuillez remplir tous les champs obligatoires: Nom, Prénom, Date de naissance, Genre et Statut d'emploi.");
            return;
        }

        Resident resident;
        resident.setCin(cin);
        resident.setNom(nom);
        resident.setPrenom(prenom);
        resident.setSexe(sexe);
        resident.setDateNaissance(dateNaissance);
        resident.setAdresse(adresse);
        resident.setAdresseMail(email);
        resident.setTelephone(telephone);
        resident.setStatutEmploi(statutEmploi);
        resident.setStatut("Actif");

        if (resident.modifier()) {
            QMessageBox::information(this, "Succès", "Le résident a été mis à jour avec succès!");
            refreshResidentList();
            showResidentDetails(cin.toInt());
        } else {
            QMessageBox::critical(this, "Erreur", "La mise à jour du résident a échoué.");
        }
    });

    // Center the form
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(formContainer);
    centerLayout->addStretch();

    mainLayout->addWidget(headerFrame);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();
    return editWidget;
}

// ====================================================================
// SLOTS IMPLEMENTATIONS
// ====================================================================

void MainWindow::showAddMemberForm()
{
    mainStack->setCurrentIndex(1);
}

void MainWindow::showResidentsList()
{
    // Clear search when returning to list
    if (searchBar) {
        searchBar->clear();
    }

    // Ensure the list is fresh when navigating back
    refreshResidentList();
    mainStack->setCurrentIndex(0);
}

void MainWindow::onTableItemClicked(QTableWidgetItem *item)
{
    if (item) {
        // UPDATE: Retrieve CIN from the first column instead of ID
        QString residentCin = residentsTableWidget->item(item->row(), 0)->text();
        // For now, we'll convert CIN to int for compatibility, but you might want to change this later
        showResidentDetails(residentCin.toInt());
    }
}

void MainWindow::showResidentDetails(int residentId)
{
    // Before showing the details, we must remove any previous details widget (at index 2)
    if (mainStack->count() > 2) {
        QWidget* oldWidget = mainStack->widget(2);
        mainStack->removeWidget(oldWidget);
        delete oldWidget;
    }

    QWidget* detailsWidget = createResidentDetailsWidget(residentId);
    mainStack->addWidget(detailsWidget);
    mainStack->setCurrentIndex(2); // New widget is at index 2
}

void MainWindow::showEditResidentForm(QString cin)
{
    // Remove any previous edit widget
    if (mainStack->count() > 3) {
        QWidget* oldWidget = mainStack->widget(3);
        mainStack->removeWidget(oldWidget);
        delete oldWidget;
    }

    QWidget* editWidget = createEditResidentWidget(cin);
    mainStack->addWidget(editWidget);
    mainStack->setCurrentIndex(3); // Edit widget at index 3
}
