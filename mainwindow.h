#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>

// Forward declarations pour QtCharts
class QChartView;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onDisplayAllEmployes();

public slots:
    void onTrierParSalaire();

private slots:
    void onAddEmploye();
    void onDisplayEmploye();
    void onDeleteEmploye();
    void onUpdateEmploye();
    void onTableRowSelected(int row, int column);
    void clearFields();
    void onSearchEmploye();
    void refreshEmployeTable();
    void onTabChanged(int index);
    void exportPdf();

private:
    Ui::MainWindow *ui;

    // Variable pour suivre l'onglet actuel
    int currentTabIndex = 0;

    // Fonctions pour les statistiques
    void createStatisticsWidget();
    void updateStatisticsTab();
    void updateStatistics();
    void autoRefreshStatistics(); // 🆕 Nouvelle fonction
    QGroupBox* createStatCard(const QString &title, const QString &value, const QString &color);
    QChartView* createPresenceChart();
    QChartView* createSalaryByJobChart();
};

#endif // MAINWINDOW_H
