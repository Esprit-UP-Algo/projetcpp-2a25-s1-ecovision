#ifndef LOCALWINDOW_H
#define LOCALWINDOW_H

#include <QDialog>
#include <QSqlQueryModel>

class Local;

namespace Ui {
class Dialog;
}

class LocalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LocalWindow(QWidget *parent = nullptr);
    ~LocalWindow();

private slots:
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();

private:
    Ui::Dialog *ui;
    QSqlQueryModel *model;

    bool createLocalTable();
    void afficherTable();
    void clearInputs();
};

#endif // LOCALWINDOW_H
