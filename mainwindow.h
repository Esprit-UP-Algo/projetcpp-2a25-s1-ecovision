#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <parking.h>
#include "arduino.h"

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
    QSortFilterProxyModel *proxy;

    ~MainWindow();

private slots:
    void on_ajouterclient_clicked();

    void on_modifierclient_clicked();

    void on_supprclient_clicked();
    void on_backup_clicked();
    void on_import_2_clicked();
    void on_exportButton_clicked();
    void on_afficherclient_clicked();
    void on_triId_clicked();
    void on_triNom_clicked();
    void on_triCapacite_clicked();
    void on_rechclient_2_clicked();
    void on_exportPdf_clicked();
    void generateQrCodes();


    void loadParkingsIntoCombo();
    void readSerial();
    void on_btnEnvoyerEtat_clicked();


private:
    Ui::MainWindow *ui;
    Parking parking;
    Arduino A; // objet temporaire
    QByteArray data; // variable contenant les données reçues
    void initArduino();
    void initDatabase();
    void saveToDatabase(const QString &parkingName,
                        const QString &parkingLocation,
                        bool isFree);
};
#endif // MAINWINDOW_H
