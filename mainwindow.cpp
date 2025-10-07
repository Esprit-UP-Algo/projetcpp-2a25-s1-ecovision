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

// ====================================================================
// CONSTRUCTOR
// ====================================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , sidebar(nullptr)
    , mainStack(nullptr)
    , addMemberBtn(nullptr)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    // --- Window Setup ---
    this->setFixedSize(1200, 800);

    // ---------------- Load Inter font ----------------
    int fontId = QFontDatabase::addApplicationFont(":/fonts/Inter-Regular.ttf");
    QString interFamily = (fontId != -1 && !QFontDatabase::applicationFontFamilies(fontId).isEmpty())
                              ? QFontDatabase::applicationFontFamilies(fontId).at(0)
                              : "Segoe UI";

    // Global font
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

    // Buttons
    sidebarLayout->addWidget(createSidebarButton("Gestion Employés"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Locaux"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Ressources"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Réclamations"));
    sidebarLayout->addWidget(createSidebarButton("Gestion Résidents"));

    sidebarLayout->addStretch();

    // ---------------- Main Stacked Widget ----------------
    mainStack = new QStackedWidget(centralWidget);

    // Create both pages
    QWidget* residentsListWidget = createResidentsListWidget();
    QWidget* addMemberWidget = createAddMemberWidget();

    // Add pages to stacked widget
    mainStack->addWidget(residentsListWidget); // Index 0
    mainStack->addWidget(addMemberWidget);     // Index 1

    // ---------------- Add to main layout ----------------
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainStack);
}

// ====================================================================
// HELPER FUNCTIONS IMPLEMENTATIONS
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

    // Left: Filter Section (Filtrer par Tous)
    QLabel* filterLabel = new QLabel("Filtrer par");
    filterLabel->setStyleSheet("color: #6B7280; font-size: 14px;");

    // Styled filter button
    QComboBox* filterButton = new QComboBox();
    filterButton->addItems({"Nom", "âge", "Adresse"});
    filterButton->setCurrentIndex(0);
    filterButton->setFixedSize(60, 30);
    filterButton->setStyleSheet("QPushButton { border: 1px solid #22252a; border-radius: 6px; background-color: white; }"
                                "QPushButton::menu-indicator { image: none; }");

    // Right: Search Bar
    QLineEdit* searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Chercher dans les résidents");
    searchBar->setFixedHeight(35);
    searchBar->setStyleSheet("QLineEdit { padding: 5px 10px; border: 1px solid #E5E7EB; border-radius: 6px; background-color: white; color: #111827;}");

    layout->addStretch(1);
    layout->addWidget(filterLabel);
    layout->addWidget(filterButton);
    layout->addWidget(searchBar);

    searchBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->setStretchFactor(searchBar, 2);
    layout->setStretchFactor(filterButton, 0);

    return frame;
}

QWidget* MainWindow::createStatsWidget()
{
    QWidget* statsWidget = new QWidget();
    statsWidget->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");

    QVBoxLayout* layout = new QVBoxLayout(statsWidget);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);

    // Title
    QLabel* title = new QLabel("📊 Statistiques Résidents");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #111827;");
    layout->addWidget(title);

    // Create a pie chart for age distribution
    QChart* chart = new QChart();
    chart->setBackgroundRoundness(10);
    chart->setBackgroundBrush(QBrush(QColor(255, 255, 255)));
    chart->setTitle("Répartition par âge");
    chart->setTitleBrush(QBrush(QColor(79, 79, 79)));
    chart->setTitleFont(QFont("Inter", 10, QFont::Bold));

    // Create pie series
    QPieSeries* series = new QPieSeries();

    // Age groups with colors
    QList<QPair<QString, int>> ageData = {
        {"18-25 ans", 8},
        {"26-35 ans", 15},
        {"36-45 ans", 12},
        {"46-60 ans", 7},
        {"60+ ans", 5}
    };

    QList<QColor> colors = {
        QColor("#5cb5f2"),  // Blue
        QColor("#34d399"),  // Green
        QColor("#f59e0b"),  // Orange
        QColor("#ef4444"),  // Red
        QColor("#8b5cf6")   // Purple
    };

    for (int i = 0; i < ageData.size(); ++i) {
        QPieSlice* slice = series->append(ageData[i].first, ageData[i].second);
        slice->setColor(colors[i]);
        slice->setLabelVisible(true);
        slice->setLabelColor(QColor(55, 65, 81));
        slice->setLabelFont(QFont("Inter", 9));
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabel(QString("%1\n%2%").arg(ageData[i].first).arg(ageData[i].second));
    }

    chart->addSeries(series);
    chart->legend()->setVisible(false);
    chart->setAnimationOptions(QChart::AllAnimations);

    // Chart view
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedHeight(200);
    chartView->setStyleSheet("background: transparent; border: none;");

    layout->addWidget(chartView);

    return statsWidget;
}

QWidget* MainWindow::createMapWidget()
{
    QWidget* mapWidget = new QWidget();
    mapWidget->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");

    QVBoxLayout* layout = new QVBoxLayout(mapWidget);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(10);

    // Title
    QLabel* title = new QLabel("🗺️ Carte des Résidents");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #111827;");
    layout->addWidget(title);

    // Interactive map visualization
    QWidget* mapVisualization = new QWidget();
    mapVisualization->setStyleSheet(
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #dbeafe, stop:1 #e0f2fe);"
        "border-radius: 8px;"
        "border: 2px solid #bfdbfe;"
        );
    mapVisualization->setFixedHeight(180);

    QGridLayout* mapLayout = new QGridLayout(mapVisualization);
    mapLayout->setContentsMargins(15, 15, 15, 15);
    mapLayout->setHorizontalSpacing(10);
    mapLayout->setVerticalSpacing(10);

    // Create zones with residents
    struct MapZone {
        QString name;
        int row;
        int col;
        int residents;
        QString color;
    };

    QList<MapZone> zones = {
        {"Nord", 0, 1, 12, "#5cb5f2"},
        {"Sud", 2, 1, 8, "#34d399"},
        {"Est", 1, 2, 5, "#f59e0b"},
        {"Ouest", 1, 0, 7, "#ef4444"},
        {"Centre", 1, 1, 15, "#8b5cf6"}
    };

    for (const auto& zone : zones) {
        QFrame* zoneFrame = new QFrame();
        zoneFrame->setStyleSheet(
            QString(
                "QFrame {"
                "    background: %1;"
                "    border: 2px solid white;"
                "    border-radius: 6px;"
                "}"
                ).arg(zone.color)
            );
        zoneFrame->setFixedSize(80, 50);

        QVBoxLayout* zoneLayout = new QVBoxLayout(zoneFrame);
        zoneLayout->setContentsMargins(5, 3, 5, 3);
        zoneLayout->setSpacing(1);

        QLabel* nameLabel = new QLabel(zone.name);
        nameLabel->setStyleSheet("color: white; font-size: 10px; font-weight: bold;");
        nameLabel->setAlignment(Qt::AlignCenter);

        QLabel* residentLabel = new QLabel(QString("%1 rés.").arg(zone.residents));
        residentLabel->setStyleSheet("color: white; font-size: 9px;");
        residentLabel->setAlignment(Qt::AlignCenter);

        zoneLayout->addWidget(nameLabel);
        zoneLayout->addWidget(residentLabel);

        mapLayout->addWidget(zoneFrame, zone.row, zone.col);
    }

    layout->addWidget(mapVisualization);

    return mapWidget;
}

QTableWidget* MainWindow::createStaffTable(QWidget *parent)
{
    // Create table with only 1 row for our single resident
    QTableWidget* table = new QTableWidget(1, 5, parent);
    table->setHorizontalHeaderLabels({"ID", "Nom", "Prénom", "Date de naissance", "Adresse"});

    // --- Styling and Configuration ---
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setStretchLastSection(true);

    table->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: white; border-bottom: 1px solid #E5E7EB; font-weight: bold; padding: 10px 0; color: #6B7280; }");

    table->setShowGrid(false);
    table->setFrameShape(QFrame::NoFrame);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    // Make table clickable with hover effects
    table->setStyleSheet(
        "QTableWidget {"
        "    padding: 0 10px;"
        "    background-color: white;"
        "    color: #2D3748;"  // Change text color for entire table (dark gray)
        "}"
        "QTableWidget::item {"
        "    padding: 10px 0;"
        "    border-bottom: 1px solid #F3F4F6;"
        "    color: #2D3748;"  // Ensure item text color
        "}"
        "QTableWidget::item:hover {"
        "    background-color: #F3F4F6;"
        "    cursor: pointer;"
        "    color: #2D3748;"  // Text color on hover
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #5cb5f2;"
        "    color: white;"    // White text when selected
        "}"
        );

    // Connect the item click signal
    connect(table, &QTableWidget::itemClicked, this, &MainWindow::onTableItemClicked);

    // --- Column Widths and Sizing ---
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    table->setColumnWidth(0, 40);

    table->verticalScrollBar()->setStyleSheet("QScrollBar { width: 0px; }");

    // --- Populate with only 1 resident ---
    QStringList firstNames = {"Mohsen"};
    QStringList lastNames = {"Bouriga"};
    QStringList birthDates = {"15/03/1985"};
    QStringList addresses = {"123 Rue De Dcha, Tunis"};

    // Only populate the first row
    for (int i = 0; i < table->rowCount(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));
        table->setItem(i, 1, new QTableWidgetItem(lastNames.at(i)));
        table->setItem(i, 2, new QTableWidgetItem(firstNames.at(i)));
        table->setItem(i, 3, new QTableWidgetItem(birthDates.at(i)));
        table->setItem(i, 4, new QTableWidgetItem(addresses.at(i)));

        for(int j = 0; j < table->columnCount(); ++j) {
            if (table->item(i, j)) {
                table->item(i, j)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            }
        }
    }

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

    // Left: Title
    QLabel* title = new QLabel("Résidents");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #111827;");

    // Right: Add Member Button
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

    // Add to title layout
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

    // Create stats and map widgets
    QWidget* statsWidget = createStatsWidget();
    QWidget* mapWidget = createMapWidget();

    // Add to layout with stretch factors to split available space
    widgetsLayout->addWidget(statsWidget, 1); // 1 part for stats
    widgetsLayout->addWidget(mapWidget, 1);   // 1 part for map (equal splitting)

    // Add components to the main content layout
    contentLayout->addWidget(titleRow);
    contentLayout->addWidget(headerFrame);
    contentLayout->addWidget(tableContainer);
    contentLayout->addWidget(widgetsContainer); // Add the widgets below the table
    contentLayout->addStretch();

    return residentsWidget;
}

QWidget* MainWindow::createAddMemberWidget()
{
    // Your existing createAddMemberWidget code remains exactly the same
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

    // Simple form content
    QFrame* formContainer = new QFrame();
    formContainer->setStyleSheet("background-color: white; border-radius: 12px; border: 1px solid #E5E7EB;");
    formContainer->setMaximumWidth(800);

    QVBoxLayout* formLayout = new QVBoxLayout(formContainer);
    formLayout->setContentsMargins(40, 30, 40, 30);
    formLayout->setSpacing(25);

    // Form Title
    QLabel* formTitle = new QLabel("Informations du résident");
    formTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-bottom: 10px;");
    formLayout->addWidget(formTitle);

    // Personal Information Section
    QLabel* personalInfoLabel = new QLabel("Informations personnelles");
    personalInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(personalInfoLabel);

    // Name Row
    QHBoxLayout* nameLayout = new QHBoxLayout();
    nameLayout->setSpacing(15);

    QLineEdit* firstNameField = new QLineEdit();
    firstNameField->setPlaceholderText("Prénom");
    firstNameField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}");
    firstNameField->setMinimumHeight(45);

    QLineEdit* lastNameField = new QLineEdit();
    lastNameField->setPlaceholderText("Nom");
    lastNameField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}");
    lastNameField->setMinimumHeight(45);

    nameLayout->addWidget(firstNameField);
    nameLayout->addWidget(lastNameField);
    formLayout->addLayout(nameLayout);

    // Contact Information
    QLabel* contactInfoLabel = new QLabel("Informations de contact");
    contactInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(contactInfoLabel);

    QLineEdit* emailField = new QLineEdit();
    emailField->setPlaceholderText("Adresse email");
    emailField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}");
    emailField->setMinimumHeight(45);
    formLayout->addWidget(emailField);

    QLineEdit* phoneField = new QLineEdit();
    phoneField->setPlaceholderText("Numéro de téléphone");
    phoneField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0;}");
    phoneField->setMinimumHeight(45);
    formLayout->addWidget(phoneField);

    // Address Information
    QLabel* addressInfoLabel = new QLabel("Adresse");
    addressInfoLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #374151; margin-top: 10px;");
    formLayout->addWidget(addressInfoLabel);

    QLineEdit* addressField = new QLineEdit();
    addressField->setPlaceholderText("Adresse complète");
    addressField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0; }");
    addressField->setMinimumHeight(45);
    formLayout->addWidget(addressField);

    // Date of Birth and Gender Row
    QHBoxLayout* dobGenderLayout = new QHBoxLayout();
    dobGenderLayout->setSpacing(15);

    QLineEdit* dobField = new QLineEdit();
    dobField->setPlaceholderText("Date de naissance (JJ/MM/AAAA)");
    dobField->setStyleSheet("QLineEdit { padding: 12px; border: 1px solid #D1D5DB; border-radius: 6px; font-size: 14px; color: #7a88a0; }");
    dobField->setMinimumHeight(45);

    QComboBox* genderCombo = new QComboBox();
    genderCombo->addItems({"Genre", "Homme", "Femme", "Autre"});
    genderCombo->setCurrentIndex(0);

    genderCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 12px;"
        "    border: 1px solid #D1D5DB;"
        "    border-radius: 6px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #111827;"
        "}"
        "QComboBox:focus {"
        "    border-color: #5cb5f2;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 20px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 5px solid #6B7280;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #D1D5DB;"
        "    border-radius: 6px;"
        "    background-color: white;"
        "    selection-background-color: #5cb5f2;"
        "    color: #111827;"
        "    padding: 8px;"
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        "    background-color: #5cb5f2;"
        "    color: white;"
        "}"
        );

    dobGenderLayout->addWidget(dobField);
    dobGenderLayout->addWidget(genderCombo);
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

    // Connect buttons
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::showResidentsList);
    connect(submitButton, &QPushButton::clicked, [this, firstNameField, lastNameField, emailField, phoneField, addressField, dobField, genderCombo](){
        if (firstNameField->text().isEmpty() || lastNameField->text().isEmpty()) {
            qDebug() << "Veuillez remplir les champs obligatoires";
            return;
        }

        qDebug() << "Nouveau résident ajouté:";
        qDebug() << "Prénom:" << firstNameField->text();
        qDebug() << "Nom:" << lastNameField->text();
        qDebug() << "Email:" << emailField->text();
        qDebug() << "Téléphone:" << phoneField->text();
        qDebug() << "Adresse:" << addressField->text();
        qDebug() << "Date de naissance:" << dobField->text();
        qDebug() << "Genre:" << genderCombo->currentText();

        firstNameField->clear();
        lastNameField->clear();
        emailField->clear();
        phoneField->clear();
        addressField->clear();
        dobField->clear();
        genderCombo->setCurrentIndex(0);

        showResidentsList();
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

QWidget* MainWindow::createResidentDetailsWidget(int residentId)
{
    // Your existing createResidentDetailsWidget code remains exactly the same
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
    editButton->setFixedSize(120, 35);
    editButton->setCursor(Qt::PointingHandCursor);
    editButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #f59e0b;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #d97706;"
        "}"
        );

    QPushButton* deleteButton = new QPushButton("🗑️ Supprimer");
    deleteButton->setFixedSize(120, 35);
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #ef4444;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-weight: 500;"
        "    font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #dc2626;"
        "}"
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

    // Resident ID
    QLabel* residentIdLabel = new QLabel("Résident #" + QString::number(residentId));
    residentIdLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #5cb5f2; margin-bottom: 10px;");
    contentLayout->addWidget(residentIdLabel);

    // Personal Information Section
    QLabel* personalInfoLabel = new QLabel("Informations personnelles");
    personalInfoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #111827; margin-bottom: 20px;");
    contentLayout->addWidget(personalInfoLabel);

    // Create info grid
    QGridLayout* infoGrid = new QGridLayout();
    infoGrid->setSpacing(20);
    infoGrid->setColumnMinimumWidth(0, 150);
    infoGrid->setColumnStretch(1, 1);

    // Data for our single resident
    QStringList labels = {"Nom complet:", "Date de naissance:", "Genre:", "Email:", "Téléphone:", "Adresse:", "Date d'inscription:"};

    QStringList firstNames = {"Mohsen"};
    QStringList lastNames = {"Bouriga"};
    QStringList birthDates = {"15/03/1985"};
    QStringList emails = {"mohsen.bouriga@email.com"};
    QStringList phones = {"56943765"};
    QStringList addresses = {"123 Rue De Dcha, Tunis"};

    int index = residentId - 1;
    // Add bounds checking
    if (index < 0) index = 0;
    if (index >= firstNames.size()) index = firstNames.size() - 1;

    QStringList values = {
        firstNames.at(index) + " " + lastNames.at(index),
        birthDates.at(index),
        "Homme",
        emails.at(index),
        phones.at(index),
        addresses.at(index),
        "15/01/2024"
    };

    for (int i = 0; i < labels.size(); ++i) {
        QLabel* label = new QLabel(labels[i]);
        label->setStyleSheet("font-weight: 600; color: #374151; font-size: 14px;");

        QLabel* value = new QLabel(values[i]);
        value->setStyleSheet("color: #6B7280; font-size: 14px; padding: 5px 0; height: 25px;");
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

    // Connect action buttons
    connect(editButton, &QPushButton::clicked, [this, residentId]() {
        qDebug() << "Modifier le résident:" << residentId;
    });

    connect(deleteButton, &QPushButton::clicked, [this, residentId]() {
        qDebug() << "Supprimer le résident:" << residentId;
    });

    mainLayout->addWidget(headerFrame);
    mainLayout->addWidget(contentContainer);
    mainLayout->addStretch();

    return detailsWidget;
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
    mainStack->setCurrentIndex(0);
}

void MainWindow::onTableItemClicked(QTableWidgetItem *item)
{
    if (item) {
        showResidentDetails(item->row());
    }
}

void MainWindow::showResidentDetails(int row)
{
    int residentId = row + 1;
    QWidget* detailsWidget = createResidentDetailsWidget(residentId);

    if (mainStack->count() > 2) {
        QWidget* oldWidget = mainStack->widget(2);
        mainStack->removeWidget(oldWidget);
        delete oldWidget;
    }

    mainStack->addWidget(detailsWidget);
    mainStack->setCurrentIndex(2);
}
