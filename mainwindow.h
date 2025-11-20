#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;
class QSqlQueryModel;

// nouveaux forward declarations pour la carte
class QGraphicsScene;
class QGraphicsEllipseItem;
class QGraphicsPixmapItem;
class QModelIndex;
class QPointF;
class QStandardItemModel;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    /* ---- Gestion LOCAL ---- */
    void on_ajouterButton_clicked();
    void on_modifierButton_clicked();
    void on_supprimerButton_clicked();
    void on_rechercherButton_clicked();     // Recherche local par ID (QLineEdit l5)
    void on_trierButton_clicked();          // Tri par prix
    void on_pb_pdf_clicked();               // PDF des locaux non disponibles
    void on_pb_historiqueService_clicked(); // Historique des modifications (dernier mois)
    void on_calenderier_clicked();          // Afficher les périodes de LOCATION dans calendarWidget

    /* ---- Statistiques ---- */
    void on_statistiqueButton_2_clicked();  // Camembert dispo / non dispo dans verticalLayout
    void on_statistiqueButton_clicked();    // Stat par ville dans listWidget
    void on_statistiqueButton_3_clicked();  // <<< NOUVEAU

    /* ---- Gestion RECLAMATION ---- */
    void on_pushButton_13_clicked();        // Ajouter réclamation
    void on_pushButton_12_clicked();        // Supprimer réclamation
    void on_pushButton_17_clicked();        // Modifier réclamation
    void syncReclamSelectionToForm();       // Sélection tableau -> formulaire
    void on_pushButton_8_clicked();        // <<< nouveau slot recherche par NOM_RESIDENT
    void on_pushButton_9_clicked();        // <<< nouveau : tri par date
    void on_pushButton_10_clicked();   // export PDF réclamations par état

    void on_statistiqueButton_11_clicked();   // <<< nouveau
    /* ---- Mapping tableView -> graphicsView ---- */
    void onTableCurrentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

private:
    void setupModelAndView();               // Init tableView + modèle SQL Local
    void refreshView();                     // Recharger la liste des locaux
    void loadReclamationsIntoTable();       // Charger la table RECLAMATION
    void on_statistiqueButton_10_clicked();   // statistique par nom de résident

    // helpers pour la carte
    void setupMapView();                    // Init graphicsView + scène + marqueur
    QPointF localPositionForIndex(const QModelIndex &index) const;

    Ui::MainWindow* ui;
    QSqlTableModel* m_model = nullptr;      // pas utilisé actuellement, mais gardé
    QSqlQueryModel* m_view  = nullptr;      // Modèle pour tableView (LOCAL)

    // membres pour la carte
    QGraphicsScene*       m_scene  = nullptr;
    QGraphicsPixmapItem*  m_mapItem = nullptr;
    QGraphicsEllipseItem* m_marker = nullptr;

    // modèle pour l'historique
    QStandardItemModel*   m_histModel = nullptr;

};


#endif // MAINWINDOW_H
