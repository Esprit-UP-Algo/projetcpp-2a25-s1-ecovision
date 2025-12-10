#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QLineEdit>
#include <QFontDatabase>
#include <QStackedWidget>
#include <QGridLayout>
#include <QTableWidgetItem> // Added for the onTableItemClicked slot
#include <QComboBox> // Added for filter combo box
#include <QDateEdit> // Added for date input
#include <QNetworkAccessManager> // Added for email functionality
#include <QNetworkReply> // Added for email functionality
#include <QJsonObject>    // Add this line
#include <QJsonDocument>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAddMemberForm();
    void showResidentsList();
    void showResidentDetails(int residentId);
    void onTableItemClicked(QTableWidgetItem *item);
    void showEditResidentForm(QString cin);
    void performSearch(); // Added for search functionality
    void onEmailSent(QNetworkReply* reply); // Added for email response handling

private:
    // UI elements
    QWidget *centralWidget;
    QFrame *sidebar;
    QStackedWidget *mainStack;
    QPushButton *addMemberBtn;
    QWidget *statsWidget;
    QWidget *employmentChartWidget;

    // Core Fix: Pointer to the table widget for dynamic data refresh
    QTableWidget *residentsTableWidget;

    // Search functionality components
    QLineEdit *searchBar;
    QComboBox *filterComboBox;

    // Email functionality components
    QNetworkAccessManager* networkManager;

    // Setup methods
    void setupUI();
    void setupEmailConfig();
    void refreshResidentList();
    void refreshCharts();

    // Email method
    void sendWelcomeEmail(const QString& email, const QString& firstName, const QString& lastName);

    // Helper methods
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath = "");
    QFrame* createHeaderFrame();
    QTableWidget* createStaffTable(QWidget *parent = nullptr);
    QFrame* createPaginationFrame();
    QWidget* createResidentsListWidget();
    QWidget* createAddMemberWidget();
    QWidget* createResidentDetailsWidget(int residentId);
    QWidget* createEditResidentWidget(QString cin);
    QWidget* createStatsWidget();
    QWidget* createEmploymentChartWidget();
    QWidget* createEmploymentChartContent();
    QWidget* createAgeDistributionChart();
};

#endif // MAINWINDOW_H
