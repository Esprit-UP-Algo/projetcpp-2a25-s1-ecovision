#ifndef EMPLOYEWINDOW_H
#define EMPLOYEWINDOW_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class Dialog;  // This matches your UI file
}

class employewindow : public QDialog
{
    Q_OBJECT

public:
    explicit employewindow(QWidget *parent = nullptr);
    ~employewindow();

private slots:
    void onAddEmploye();
    void onDeleteEmploye();
    void onUpdateEmploye();
    void onSearchEmploye();
    void onTrierParSalaire();
    void onTableRowSelected(int row, int column);
    void exportPdf();  // ADDED this line

private:
    Ui::Dialog *ui;

    // Helper functions
    void clearFields();
    bool isDigitsOnly(const QString &s);
    bool isValidDecimal(const QString &s);
};

#endif // EMPLOYEWINDOW_H
