#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QModelIndex>
#include <QPointF>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlQueryModel;
class QGraphicsScene;
class QGraphicsEllipseItem;
class QGraphicsPixmapItem;
class QStandardItemModel;
class SMSManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ---- Gestion des locaux ----
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void on_supprimerButton_clicked();
    void on_rechercherButton_clicked();
    void on_calenderier_clicked();
    void on_trierButton_clicked();
    void on_pb_pdf_clicked();
    void on_pb_historiqueService_clicked();

    // ---- Réclamations ----
    void on_pushButton_13_clicked();  // ajouter réclamation
    void on_pushButton_12_clicked();  // supprimer réclamation
    void on_pushButton_17_clicked();  // modifier réclamation
    void on_pushButton_8_clicked();   // recherche par état
    void on_pushButton_9_clicked();   // tri par état
    void on_pushButton_10_clicked();  // PDF par état

    // ---- Statistiques ----
    void on_statistiqueButton_clicked();
    void on_statistiqueButton_2_clicked();
    void on_statistiqueButton_3_clicked();
    void on_statistiqueButton_10_clicked();
    void on_statistiqueButton_11_clicked();

    // ---- Sélection table locaux / réclamations ----
    void onTableCurrentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);
    void syncReclamSelectionToForm();

    // ---- Callback SMS ----
    void onSmsSent(bool success, const QString &message);
    void onSmsError(const QString &error);

    // ---- Bouton de test SMS ----
    void on_pushButton_testSms_clicked();

private:
    // Locaux
    void setupModelAndView();
    void refreshView();
    void setupMapView();
    QPointF localPositionForIndex(const QModelIndex &index) const;

    // Réclamations
    void loadReclamationsIntoTable();
    void showReclamationDeadlinePopup(int code,
                                      const QString &nom,
                                      const QString &delai);

    // Bannière SMS dans la fenêtre Qt
    void showSmsBanner(const QString &text, bool success);

    // Notification Windows via QSystemTrayIcon
    void showReclamationNotification(const QString &nomResident,
                                     const QString &codeReclamation,
                                     const QString &delai);

private:
    Ui::MainWindow *ui;

    // Locaux
    QSqlQueryModel       *m_view;
    QGraphicsScene       *m_scene;
    QGraphicsEllipseItem *m_marker;
    QGraphicsPixmapItem  *m_mapItem;

    // Historique
    QStandardItemModel   *m_histModel;

    // SMS
    SMSManager           *m_smsManager;

    // Icône de notification (systray Windows)
    QSystemTrayIcon      *m_trayIcon;
};

#endif // MAINWINDOW_H
