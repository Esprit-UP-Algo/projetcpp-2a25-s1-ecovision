#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;
class QSqlQueryModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void on_supprimerButton_clicked();
    void on_pushButton_13_clicked();   // Ajouter réclamation
    void on_pushButton_12_clicked();
    void on_pushButton_17_clicked();      // Modifier réclamation (UPDATE)
    void syncReclamSelectionToForm();
private:
    void setupModelAndView();
    void refreshView();
    void loadReclamationsIntoTable();

    Ui::MainWindow* ui;
    QSqlTableModel* m_model = nullptr;
    QSqlQueryModel* m_view  = nullptr;
};

#endif // MAINWINDOW_H
