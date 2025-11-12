#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_ajouter_clicked();
    void on_btn_supprimer_clicked();
    void on_btn_modifier_clicked();
    void on_btn_afficher_clicked();
    void on_btn_recherche_clicked();
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;

    // Helper methods
    QString getSelectedType() const;
    void afficherTable();
    void clearInputs();
    bool createResourceTable();
    bool checkTableExists();
};

#endif // MAINWINDOW_H
