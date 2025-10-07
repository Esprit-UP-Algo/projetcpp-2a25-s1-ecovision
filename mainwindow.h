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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAddMemberForm();
    void showResidentsList();
    void showResidentDetails(int row);
    void onTableItemClicked(QTableWidgetItem *item);

private:
    // UI elements
    QWidget *centralWidget;
    QFrame *sidebar;
    QStackedWidget *mainStack;
    QPushButton *addMemberBtn;

    // Helper methods
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath = "");
    QFrame* createHeaderFrame();
    QTableWidget* createStaffTable(QWidget *parent = nullptr);
    QFrame* createPaginationFrame();
    QWidget* createResidentsListWidget();
    QWidget* createAddMemberWidget();
    QWidget* createResidentDetailsWidget(int residentId);
    QWidget* createStatsWidget();
    QWidget* createMapWidget();

    void setupUI();
};

#endif // MAINWINDOW_H
