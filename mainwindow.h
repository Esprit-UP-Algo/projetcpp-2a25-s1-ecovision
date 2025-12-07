#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Employee Tab Slots
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btntrier_clicked();
    void on_btnpdf_clicked();
    void on_tableWidget_3_cellClicked(int row, int column);
    void on_cin_4_textChanged(const QString &text);

    // Resource Tab Slots
    void on_btn_ajouter_clicked();
    void on_btn_modifier_clicked();
    void on_btn_supprimer_clicked();
    void on_btn_recherche_clicked();
    void on_btn_afficher_id_clicked();
    void on_btn_afficher_prix_clicked();
    void on_pushButton_4_clicked();
    void on_btn_pdf_avance_clicked();
    void on_btn_qrcode_clicked();
    void on_statsButton_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_btn_afficher_clicked();

    // Navigation
    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    // Employee Methods
    void loadEmployeeData();
    void clearEmployeeFields();
    bool validateEmployeeInput();
    void exportEmployeePDF();

    // Resource Methods
    void loadResourceData();
    void loadResourceStats();
    void clearResourceFields();
    bool validateResourceInput();
    QString getResourceType() const;

    // Resource helper methods
    void chargerDonneesParID(int id);
    void afficherStatistiques();
    int getDefaultResidentId();
    void genererPDFAvance();
    void genererPDFGaranti(int residentIdInput);
    bool createResourceTable();
};

#endif // MAINWINDOW_H
