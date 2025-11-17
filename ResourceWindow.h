#ifndef RESOURCEWINDOW_H
#define RESOURCEWINDOW_H

#include <QDialog>

// Forward declaration - MUST match your UI file class name
namespace Ui {
class Dialog;  // Changed from ResourceWindow to Dialog
}

class ResourceWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResourceWindow(QWidget *parent = nullptr);
    ~ResourceWindow();

private slots:
    void on_btn_ajouter_clicked();
    void on_btn_supprimer_clicked();
    void on_btn_modifier_clicked();
    void on_btn_afficher_clicked();
    void on_btn_recherche_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_btn_afficher_id_clicked();
    void on_btn_afficher_prix_clicked();
    void on_btn_pdf_clicked();

private:
    Ui::Dialog *ui;  // Changed from ResourceWindow to Dialog
    bool createResourceTable();
    QString getSelectedType() const;
    void afficherTable();
    void clearInputs();
    void chargerDonneesParID(int id);
    void afficherStatistiques();
};

#endif // RESOURCEWINDOW_H
