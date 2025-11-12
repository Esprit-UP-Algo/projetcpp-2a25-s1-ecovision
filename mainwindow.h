#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "reclamation.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ========== NAVIGATION ==========
    void on_pushButton_6_clicked();  // Aller vers Ajouter (onglet 1)
    void on_pushButton_7_clicked();  // Aller vers Modifier (onglet 2)

    // ========== AJOUTER ==========
    void on_pushButton_9_clicked();  // Soumettre ajout

    // ========== MODIFIER ==========
    void on_lineEdit_7_textChanged(const QString &text);  // Recherche auto
    void on_pushButton_11_clicked();  // Enregistrer modification

    // ========== SUPPRIMER ==========
    void on_pushButton_12_clicked();  // Supprimer
    void on_tableWidget_cellClicked(int row, int column);  // Sélection

    // ========== TRI PAR DATE ==========
    void on_pushButton_8_clicked();  // Chercher par date

private:
    Ui::MainWindow *ui;
    Reclamation Rtmp;
    void afficherReclamations();
    void afficherDansTreeWidget(QList<Reclamation> liste);  // Afficher résultats tri
    int codeSelectionne;
};

#endif // MAINWINDOW_H
