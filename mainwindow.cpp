#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "local.h"
#include "connection.h"
#include "reclamation.h"

#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QLocale>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <optional>
#include <climits>
#include <QDate>
#include <algorithm>

#include <QStandardPaths>
#include <QDir>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>

#include <QFile>
#include <QTextStream>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>

// nouveaux includes pour la carte
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QBrush>
#include <QModelIndex>
#include <QPointF>

#include <QTextCharFormat>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtMath>
#include <QtCharts/QSplineSeries>
#include <QDebug>

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QHeaderView>




/* ---- helpers ---- */
static bool parseDateReclamation(const QString& s, QDate& out) {
    const QStringList fmts = {"dd/MM/yyyy", "yyyy-MM-dd", "dd-MM-yyyy"};
    for (const auto& f : fmts) {
        const QDate d = QDate::fromString(s.trimmed(), f);
        if (d.isValid()) { out = d; return true; }
    }
    return false;
}

static QString canonEtat(QString s) {
    s = s.trimmed();
    if (s.isEmpty()) return "OUVERTE";
    s = s.normalized(QString::NormalizationForm_D);
    s.remove(QRegularExpression(u8"[\\p{Mn}]"));
    s.replace(' ', '_');
    s.replace('-', '_');
    s = s.toUpper();

    if (s == "OUVERT" || s == "OUVERTS" || s == "OUVERTES") return "OUVERTE";
    if (s == "ENCOURS") return "EN_COURS";
    if (s == "CLOS" || s == "CLOTURE" || s == "CLOTUREE" || s == "FERME" || s == "FERMEE") return "CLOSE";
    return s;
}

static bool parsePrixRobuste(const QString& raw, double* out) {
    QString txt = raw.trimmed();
    bool ok = false;
    double v = QLocale::system().toDouble(txt, &ok);
    if (ok) {
        if (out) *out = v;
        return true;
    }

    QString n = txt;
    n.remove(' ');
    n.remove(QChar(0x00A0));
    n.remove(QChar(0x202F));
    n.replace(',', '.');
    v = n.toDouble(&ok);
    if (ok) {
        if (out) *out = v;
        return true;
    }

    static const QRegularExpression re(
        R"(([+-]?\d{1,3}(?:[ \u00A0\u202F]?\d{3})*(?:[.,]\d+)?|[+-]?\d+(?:[.,]\d+)?))");
    auto m = re.match(txt);
    if (!m.hasMatch()) return false;

    QString num = m.captured(1);
    num.remove(' ');
    num.remove(QChar(0x00A0));
    num.remove(QChar(0x202F));
    num.replace(',', '.');
    v = num.toDouble(&ok);
    if (ok && out) *out = v;
    return ok;
}

static QLineEdit* le(QWidget* root, const char* name) {
    return root->findChild<QLineEdit*>(name);
}

static QString localSelectSql()
{
    return QStringLiteral(
        "SELECT "
        "  ID_LOCAL                         AS \"ID\","
        "  TYPE_LOCAL                       AS \"Type\","
        "  VILLE                            AS \"Ville\","
        "  ADRESSE                          AS \"Adresse\","
        "  DISPONIBILITE                    AS \"Dispo\","
        "  NOM_PROPRIETAIRE                 AS \"Nom prop.\","
        "  TO_CHAR(NUMERO)                  AS \"Numéro\","
        "  TO_CHAR(PRIX, 'FM9999999990D00', 'NLS_NUMERIC_CHARACTERS=.,') AS \"Prix\","
        "  TO_CHAR(CIN_PROPRIETAIRE)        AS \"CIN\" "
        "FROM LOCAL"
        );
}

/* ---- historique des locaux ---- */
static void writeLocalHistory(const QString& action)
{
    const QString baseDirPath = QStringLiteral("C:/Users/firas/Desktop/local de pdf qt");
    QDir dir(baseDirPath);
    if (!dir.exists()) {
        QDir().mkpath(baseDirPath);
    }

    const QString path = dir.filePath("historique_local.txt");

    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    const QString datetime = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    out << datetime << " | " << action << "\n";
    file.close();
}

/* ---- MainWindow ---- */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Modèle pour l'historique dans tableView_2
    m_histModel = new QStandardItemModel(this);
    m_histModel->setColumnCount(2);
    m_histModel->setHeaderData(0, Qt::Horizontal, "Date / heure");
    m_histModel->setHeaderData(1, Qt::Horizontal, "Action");

    if (ui->tableView_2) {
        ui->tableView_2->setModel(m_histModel);
        ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView_2->setAlternatingRowColors(true);
        ui->tableView_2->horizontalHeader()->setStretchLastSection(true);
        ui->tableView_2->horizontalHeader()->setHighlightSections(false);
    }

    // connexion du bouton statistiqueButton_10
    if (ui->statistiqueButton_10) {
        connect(ui->statistiqueButton_10,
                &QPushButton::clicked,
                this,
                &MainWindow::on_statistiqueButton_10_clicked);
    } else {
        qDebug() << "ERREUR: ui->statistiqueButton_10 est NULL";
    }

    // Schéma Oracle
    {
        QSqlQuery s(Connection::db());
        s.exec("ALTER SESSION SET CURRENT_SCHEMA=FIRAS");
    }

    // Validateurs ajout
    if (auto *dispoAdd = le(this, "l4"))
        dispoAdd->setValidator(new QIntValidator(0, 1, this));

    if (auto *prixAdd = le(this, "l4_4")) {
        auto *dv = new QDoubleValidator(0.0, 1e12, 2, this);
        dv->setNotation(QDoubleValidator::StandardNotation);
        dv->setLocale(QLocale::system());
        prixAdd->setValidator(dv);
    }

    // CIN_proprietaire : exactement 8 chiffres
    if (auto *cinAdd = le(this, "l4_5")) {
        auto *v = new QRegularExpressionValidator(
            QRegularExpression("^\\d{8}$"), this);
        cinAdd->setValidator(v);
    }

    // numero : exactement 8 chiffres
    if (auto *numAdd = le(this, "l4_3")) {
        auto *v = new QRegularExpressionValidator(
            QRegularExpression("^\\d{8}$"), this);
        numAdd->setValidator(v);
    }

    // Validateurs modif
    if (auto *idLE = le(this, "lineedit_id_modif"))
        idLE->setValidator(new QIntValidator(1, INT_MAX, this));

    if (auto *prixMod = le(this, "l7")) {
        auto *dv = new QDoubleValidator(0.0, 1e12, 2, this);
        dv->setNotation(QDoubleValidator::StandardNotation);
        dv->setLocale(QLocale::system());
        prixMod->setValidator(dv);
    }

    if (auto *dispoMod = le(this, "l8"))
        dispoMod->setValidator(new QIntValidator(0, 1, this));

    // modèle + tableView (LOCAL)
    setupModelAndView();

    // préparation de la carte dans graphicsView
    setupMapView();

    // synchro sélection tableView -> formulaire + carte
    if (ui->tableView->selectionModel()) {
        connect(ui->tableView->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this,
                &MainWindow::onTableCurrentRowChanged);
    }

    // Charger les réclamations au démarrage
    loadReclamationsIntoTable();
}


MainWindow::~MainWindow() { delete ui; }

/* ---- modèle / vue Local ---- */
void MainWindow::setupModelAndView()
{
    m_view = new QSqlQueryModel(this);
    m_view->setQuery(localSelectSql(), Connection::db());

    ui->tableView->setModel(m_view);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::refreshView()
{
    if (m_view)
        m_view->setQuery(localSelectSql(), Connection::db());
}

/* ---- préparation de la carte dans graphicsView ---- */
void MainWindow::setupMapView()
{
    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    // adapte ce chemin à ton projet
    QPixmap mapPix(":/images/plan_locaux.png");

    if (mapPix.isNull()) {
        qWarning() << "Impossible de charger l'image du plan (: /images/plan_locaux.png)";
        m_scene->setSceneRect(0, 0, 800, 600);
    } else {
        m_mapItem = m_scene->addPixmap(mapPix);
        m_scene->setSceneRect(m_mapItem->boundingRect());
    }

    const int r = 6;
    m_marker = m_scene->addEllipse(-r, -r, 2 * r, 2 * r,
                                   QPen(Qt::red),
                                   QBrush(Qt::red));
    m_marker->setVisible(false);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
}

/* ---- conversion ligne de tableView -> position sur la carte ---- */
QPointF MainWindow::localPositionForIndex(const QModelIndex &index) const
{
    if (!index.isValid() || !ui->tableView || !ui->tableView->model())
        return {};

    const QAbstractItemModel *model = ui->tableView->model();
    const int row = index.row();

    const QString ville = model->index(row, 2).data().toString().trimmed().toUpper();

    if (ville == "TUNIS")   return QPointF(120, 180);
    if (ville == "SFAX")    return QPointF(260, 210);
    if (ville == "SOUSSE")  return QPointF(340, 260);
    if (ville == "NABEUL")  return QPointF(420, 190);

    return QPointF(50, 50);
}

/* ---- réaction au changement de ligne dans tableView ---- */
void MainWindow::onTableCurrentRowChanged(const QModelIndex &current,
                                          const QModelIndex & /*previous*/)
{
    if (!current.isValid() || !ui->tableView || !ui->tableView->model())
        return;

    const QVariant idv = ui->tableView->model()->index(current.row(), 0).data();
    if (auto *idLE = this->findChild<QLineEdit*>("lineedit_id_modif"))
        idLE->setText(idv.toString());

    if (!m_marker)
        return;

    const QPointF pos = localPositionForIndex(current);
    m_marker->setVisible(true);
    m_marker->setPos(pos);
    ui->graphicsView->centerOn(m_marker);
}

/* ---- Ajouter Local + LOCATION ---- */
void MainWindow::on_ajouterButton_clicked()
{
    // 1) Lire et valider DISPONIBILITE et PRIX
    bool okDispo = false, okPrix = false;
    const int dispo = le(this, "l4")->text().trimmed().toInt(&okDispo);

    double prix = 0.0;
    okPrix = parsePrixRobuste(le(this, "l4_4")->text(), &prix);

    if (!okDispo || (dispo != 0 && dispo != 1)) {
        QMessageBox::warning(this, "Champs invalides", "Disponibilité doit être 0 ou 1.");
        return;
    }
    if (!okPrix || prix < 0) {
        QMessageBox::warning(this, "Champs invalides", "Prix doit être un nombre positif.");
        return;
    }

    // 2) Lire et valider DATE_DEBUT (l4_6) et DATE_FIN (l4_7)
    QLineEdit* leDebut = le(this, "l4_6");
    QLineEdit* leFin   = le(this, "l4_7");

    if (!leDebut || !leFin) {
        QMessageBox::critical(this, "UI", "QLineEdit l4_6 ou l4_7 introuvable.");
        return;
    }

    const QString txtDebut = leDebut->text().trimmed();
    const QString txtFin   = leFin->text().trimmed();

    if (txtDebut.isEmpty() || txtFin.isEmpty()) {
        QMessageBox::warning(this, "Dates manquantes",
                             "DATE_DEBUT (l4_6) et DATE_FIN (l4_7) sont obligatoires pour la location.");
        return;
    }

    QDate dateDebut, dateFin;
    if (!parseDateReclamation(txtDebut, dateDebut)) {
        QMessageBox::warning(this, "Date début invalide",
                             "Format de DATE_DEBUT invalide. Exemples : 21/11/2024 ou 2024-11-21.");
        return;
    }
    if (!parseDateReclamation(txtFin, dateFin)) {
        QMessageBox::warning(this, "Date fin invalide",
                             "Format de DATE_FIN invalide. Exemples : 21/11/2025 ou 2025-11-21.");
        return;
    }
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Dates incohérentes",
                             "DATE_FIN ne peut pas être avant DATE_DEBUT.");
        return;
    }

    // 3) Contrôle CIN et numéro : exactement 8 chiffres
    QString cinTxt = le(this, "l4_5")->text().trimmed();
    QString numTxt = le(this, "l4_3")->text().trimmed();
    QRegularExpression rx8("^\\d{8}$");

    if (!cinTxt.isEmpty() && !rx8.match(cinTxt).hasMatch()) {
        QMessageBox::warning(this, "CIN invalide",
                             "Cin_de proprietaire doit contenir exactement 8 chiffres.");
        return;
    }

    if (!numTxt.isEmpty() && !rx8.match(numTxt).hasMatch()) {
        QMessageBox::warning(this, "Numéro invalide",
                             "Numero doit contenir exactement 8 chiffres.");
        return;
    }

    // 4) Construire l'objet Local
    Local loc(
        le(this, "l1")->text().trimmed(),
        le(this, "l2")->text().trimmed(),
        le(this, "l3")->text().trimmed(),
        dispo,
        le(this, "l4_2")->text().trimmed(),
        cinTxt,
        numTxt,
        prix
        );

    QString err;
    if (!loc.isValid(&err)) {
        QMessageBox::warning(this, "Champs invalides", err);
        return;
    }

    // 5) Insertion dans LOCAL
    if (!loc.insert(&err))  {
        QMessageBox::critical(this, "Échec insertion LOCAL", err);
        return;
    }

    // 6) Récupérer l'ID_LOCAL du local inséré
    int idLocal = 0;
    {
        QSqlQuery q(Connection::db());
        if (!q.exec("SELECT MAX(ID_LOCAL) FROM LOCAL")) {
            QMessageBox::warning(this, "Avertissement",
                                 "Local ajouté mais impossible de récupérer ID_LOCAL : "
                                     + q.lastError().text());
        } else if (q.next()) {
            idLocal = q.value(0).toInt();
        }
    }

    bool locationOK = false;

    if (idLocal <= 0) {
        QMessageBox::warning(this, "Avertissement",
                             "Local ajouté, mais ID_LOCAL introuvable. "
                             "Insertion dans LOCATION non effectuée.");
    } else {
        // 7) Insertion dans LOCATION
        // Dans ta base la colonne s'appelle ID_LOCAAL
        QSqlQuery qLoc(Connection::db());
        qLoc.prepare(
            "INSERT INTO LOCATION (ID_LOCAAL, DATE_DEBUT, DATE_FIN) "
            "VALUES (:id_local, :date_debut, :date_fin)"
            );
        qLoc.bindValue(":id_local",   idLocal);
        qLoc.bindValue(":date_debut", dateDebut);
        qLoc.bindValue(":date_fin",   dateFin);

        if (!qLoc.exec()) {
            QMessageBox::warning(this, "Avertissement",
                                 "Local ajouté, mais échec insertion dans LOCATION : "
                                     + qLoc.lastError().text());
        } else {
            locationOK = true;
        }
    }

    // 8) Historique ajout
    writeLocalHistory(
        QString("AJOUT LOCAL | Type=%1 | Ville=%2 | Adresse=%3 | Dispo=%4 | NomProp=%5 | CIN=%6 | Num=%7 | Prix=%8")
            .arg(loc.type_local)
            .arg(loc.ville)
            .arg(loc.adresse)
            .arg(loc.disponibilite)
            .arg(loc.nom_proprietaire)
            .arg(loc.cin_proprietaire)
            .arg(loc.numero)
            .arg(loc.prix)
        );

    // 9) Nettoyer les champs
    le(this, "l1")->clear();
    le(this, "l2")->clear();
    le(this, "l3")->clear();
    le(this, "l4")->clear();
    le(this, "l4_2")->clear();
    le(this, "l4_5")->clear();
    le(this, "l4_3")->clear();
    le(this, "l4_4")->clear();
    le(this, "l4_6")->clear();
    le(this, "l4_7")->clear();

    // 10) Message final
    if (locationOK) {
        QMessageBox::information(this, "Succès", "Local et période de location ajoutés.");
    } else {
        QMessageBox::information(this, "Succès partiel",
                                 "Local ajouté, mais pas la période de location.");
    }

    refreshView();
}

/* ---- Modifier Local ---- */
void MainWindow::on_modifierButton_clicked()
{
    auto *idLE    = le(this, "lineedit_id_modif");
    auto *prixLE  = le(this, "l7");
    auto *dispoLE = le(this, "l8");

    if (!idLE || !prixLE || !dispoLE) {
        QMessageBox::critical(this, "Erreur UI", "lineedit_id_modif, l7 et l8 doivent exister.");
        return;
    }

    bool okId = false;
    const int id = idLE->text().trimmed().toInt(&okId);
    if (!okId || id <= 0) {
        QMessageBox::warning(this, "ID invalide", "ID_LOCAL doit être > 0.");
        return;
    }

    const QString prixTxt  = prixLE->text().trimmed();
    const QString dispoTxt = dispoLE->text().trimmed();
    if (prixTxt.isEmpty() && dispoTxt.isEmpty()) {
        QMessageBox::information(this, "Rien à faire", "Aucun champ à modifier.");
        return;
    }

    std::optional<double> newPrix;
    std::optional<int>    newDispo;

    if (!prixTxt.isEmpty()) {
        double p = 0.0;
        if (!parsePrixRobuste(prixTxt, &p) || p < 0) {
            QMessageBox::warning(this, "Prix invalide", "Prix doit être un nombre positif.");
            return;
        }
        newPrix = p;
    }

    if (!dispoTxt.isEmpty()) {
        bool okD = false;
        const int d = dispoTxt.toInt(&okD);
        if (!okD || (d != 0 && d != 1)) {
            QMessageBox::warning(this, "Disponibilité invalide", "Utilise 0 ou 1.");
            return;
        }
        newDispo = d;
    }

    QString err;
    if (!Local::updateById(id, newPrix, newDispo, &err)) {
        QMessageBox::critical(this, "Échec modification", err);
        return;
    }

    QStringList parts;
    if (newPrix)
        parts << QString("Prix=%1").arg(*newPrix);
    if (newDispo)
        parts << QString("Dispo=%1").arg(*newDispo);

    writeLocalHistory(
        QString("MODIF LOCAL ID=%1 | %2").arg(id).arg(parts.join(" , "))
        );

    prixLE->clear();
    dispoLE->clear();

    QMessageBox::information(this, "Succès", "Local modifié.");
    refreshView();
}

/* ---- Supprimer Local ---- */
void MainWindow::on_supprimerButton_clicked()
{
    int id = 0;
    bool okId = false;

    if (auto *idLE = le(this, "lineedit_id_modif"))
        id = idLE->text().trimmed().toInt(&okId);

    if (!okId || id <= 0) {
        if (ui->tableView && ui->tableView->model()) {
            QModelIndex current = ui->tableView->currentIndex();
            if (!current.isValid() && ui->tableView->selectionModel())
                current = ui->tableView->selectionModel()->currentIndex();
            if (current.isValid()) {
                const QVariant idv = ui->tableView->model()->index(current.row(), 0).data();
                bool ok2 = false;
                const int id2 = idv.toString().trimmed().toInt(&ok2);
                if (ok2 && id2 > 0) {
                    id = id2;
                    okId = true;
                }
            }
        }
    }

    if (!okId || id <= 0) {
        QMessageBox::warning(this, "ID invalide", "ID_LOCAL doit être > 0.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              QString("Supprimer l'élément ID %1 ?").arg(id),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        return;

    QString err;
    if (!Local::deleteById(id, &err)) {
        QMessageBox::critical(this, "Échec suppression", err);
        return;
    }

    writeLocalHistory(QString("SUPPRESSION LOCAL ID=%1").arg(id));

    if (auto *idLE = le(this, "lineedit_id_modif"))
        idLE->clear();
    if (ui->tableView->selectionModel())
        ui->tableView->selectionModel()->clearSelection();

    QMessageBox::information(this, "Succès", "Suppression effectuée.");
    refreshView();
}

/* ---- Recherche Local par ID_LOCAL via l5 ---- */
void MainWindow::on_rechercherButton_clicked()
{
    QLineEdit* idLE = le(this, "l5");   // ton champ "Rechercher un local par ID"
    if (!idLE) {
        QMessageBox::critical(this, "UI", "QLineEdit 'l5' introuvable.");
        return;
    }

    QString txt = idLE->text().trimmed();

    // Si vide -> recharger toute la liste
    if (txt.isEmpty()) {
        refreshView();
        return;
    }

    // On construit le SQL dynamiquement
    QString sql = localSelectSql() + " WHERE ";
    QStringList conds;

    // 1) Essayer de l'interpréter comme ID_LOCAL
    bool okId = false;
    int id = txt.toInt(&okId);
    if (okId && id > 0) {
        conds << QString("ID_LOCAL = %1").arg(id);
    }

    // 2) Recherche sur TYPE_LOCAL (LIKE, insensible à la casse)
    QString txtUpper = txt.toUpper();
    txtUpper.replace("'", "''");  // échapper les guillemets simples

    conds << QString("UPPER(TYPE_LOCAL) LIKE '%%1%'").arg(txtUpper);

    // 3) Recherche sur NOM_PROPRIETAIRE (LIKE, insensible à la casse)
    conds << QString("UPPER(NOM_PROPRIETAIRE) LIKE '%%1%'").arg(txtUpper);

    sql += conds.join(" OR ");

    m_view->setQuery(sql, Connection::db());
    ui->tableView->resizeColumnsToContents();

    if (m_view->lastError().isValid()) {
        QMessageBox::critical(this, "Recherche",
                              "Erreur SQL: " + m_view->lastError().text());
        return;
    }

    if (m_view->rowCount() == 0) {
        QMessageBox::information(this, "Recherche",
                                 "Aucun local trouvé pour « " + txt + " ».");
    }
}



/* ---- Bouton calendrier : afficher périodes de LOCATION ---- */
void MainWindow::on_calenderier_clicked()
{
    if (!ui->tableView || !ui->tableView->model()) {
        QMessageBox::critical(this, "UI", "tableView introuvable ou sans modèle.");
        return;
    }

    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Sélection",
                             "Sélectionne d'abord un local dans le tableau.");
        return;
    }

    const int row = index.row();
    bool ok = false;
    int idLocal = ui->tableView->model()->index(row, 0).data().toInt(&ok);
    if (!ok || idLocal <= 0) {
        QMessageBox::warning(this, "Erreur",
                             "ID_LOCAL invalide pour la ligne sélectionnée.");
        return;
    }

    if (!ui->calendarWidget_2) {
        QMessageBox::critical(this, "UI", "calendarWidget introuvable.");
        return;
    }

    QSqlQuery q(Connection::db());
    // colonne ID_LOCAAL dans ta table LOCATION
    q.prepare("SELECT DATE_DEBUT, DATE_FIN FROM LOCATION WHERE ID_LOCAAL = :id");
    q.bindValue(":id", idLocal);

    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Requête sur LOCATION échouée : "
                                  + q.lastError().text());
        return;
    }

    ui->calendarWidget_2->setDateTextFormat(QDate(), QTextCharFormat());

    QTextCharFormat fmt;
    fmt.setBackground(QBrush(Qt::yellow));
    fmt.setForeground(QBrush(Qt::black));

    bool aPeriode = false;

    while (q.next()) {
        QDate dDebut = q.value(0).toDate();
        QDate dFin   = q.value(1).toDate();

        if (!dDebut.isValid() || !dFin.isValid() || dFin < dDebut)
            continue;

        aPeriode = true;

        QDate d = dDebut;
        while (d <= dFin) {
            ui->calendarWidget_2->setDateTextFormat(d, fmt);
            d = d.addDays(1);
        }
    }

    if (!aPeriode) {
        QMessageBox::information(this, "Calendrier",
                                 "Aucune période de location trouvée pour ce local.");
        return;
    }

    QSqlQuery q2(Connection::db());
    q2.prepare("SELECT MIN(DATE_DEBUT) FROM LOCATION WHERE ID_LOCAAL = :id");
    q2.bindValue(":id", idLocal);
    if (q2.exec() && q2.next()) {
        QDate minDebut = q2.value(0).toDate();
        if (minDebut.isValid())
            ui->calendarWidget_2->setSelectedDate(minDebut);
    }
}

/* ---- Tri par PRIX ---- */
void MainWindow::on_trierButton_clicked()
{
    QString sql = localSelectSql() + QStringLiteral(" ORDER BY PRIX");

    m_view->setQuery(sql, Connection::db());
    ui->tableView->resizeColumnsToContents();

    if (m_view->lastError().isValid()) {
        QMessageBox::critical(this, "Tri", "Erreur SQL: " + m_view->lastError().text());
    }
}

/* ---- PDF des locaux non disponibles ---- */
void MainWindow::on_pb_pdf_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    QSqlQuery q(db);

    if (!q.exec("SELECT ID_LOCAL, TYPE_LOCAL, VILLE, ADRESSE, NOM_PROPRIETAIRE, NUMERO, PRIX "
                "FROM LOCAL "
                "WHERE DISPONIBILITE = 0 "
                "ORDER BY PRIX")) {
        QMessageBox::critical(this, "PDF", "Erreur SQL: " + q.lastError().text());
        return;
    }

    const QString baseDirPath = QStringLiteral("C:/Users/firas/Desktop/local de pdf qt");
    QDir baseDir(baseDirPath);

    if (!baseDir.exists()) {
        if (!QDir().mkpath(baseDirPath)) {
            QMessageBox::critical(this, "PDF",
                                  "Impossible de créer le dossier:\n" + baseDirPath);
            return;
        }
    }

    const QString filePath = baseDir.filePath("locaux_non_disponibles.pdf");

    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "PDF", "Impossible de créer le PDF.");
        return;
    }

    QFont font("Helvetica", 11);
    painter.setFont(font);
    QFontMetrics fm(font);

    const int margin = 60;
    const int topMargin = 80;
    const int usableWidth = pdf.width() - 2 * margin;
    const int pageBottom = pdf.height() - margin;

    int y = topMargin;

    QFont titleFont("Helvetica", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, usableWidth, 30),
                     Qt::AlignCenter, "Locaux non disponibles");
    y += 40;

    painter.setFont(font);
    const QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    painter.drawText(QRect(margin, y, usableWidth, 20),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "Généré le: " + dateStr);
    y += 40;

    const double wIdPct   = 0.07;
    const double wTypePct = 0.10;
    const double wVillePct= 0.10;
    const double wAdrPct  = 0.30;
    const double wNomPct  = 0.20;
    const double wNumPct  = 0.08;
    const double wPrixPct = 0.15;

    const int wId   = int(usableWidth * wIdPct);
    const int wType = int(usableWidth * wTypePct);
    const int wVille= int(usableWidth * wVillePct);
    const int wAdr  = int(usableWidth * wAdrPct);
    const int wNom  = int(usableWidth * wNomPct);
    const int wNum  = int(usableWidth * wNumPct);
    const int wPrix = usableWidth - (wId + wType + wVille + wAdr + wNom + wNum);

    auto drawHeaderRow = [&](int& yPos) {
        QFont headerFont("Helvetica", 12, QFont::Bold);
        painter.setFont(headerFont);

        int x = margin;
        const int headerHeight = 30;

        painter.save();
        painter.setBrush(QColor(230, 230, 230));
        painter.setPen(Qt::black);
        painter.drawRect(x, yPos, wId, headerHeight);
        painter.drawRect(x + wId, yPos, wType, headerHeight);
        painter.drawRect(x + wId + wType, yPos, wVille, headerHeight);
        painter.drawRect(x + wId + wType + wVille, yPos, wAdr, headerHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr, yPos, wNom, headerHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr + wNom, yPos, wNum, headerHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr + wNom + wNum, yPos, wPrix, headerHeight);
        painter.restore();

        painter.drawText(QRect(x, yPos, wId, headerHeight),
                         Qt::AlignCenter, "ID");
        x += wId;
        painter.drawText(QRect(x, yPos, wType, headerHeight),
                         Qt::AlignCenter, "Type");
        x += wType;
        painter.drawText(QRect(x, yPos, wVille, headerHeight),
                         Qt::AlignCenter, "Ville");
        x += wVille;
        painter.drawText(QRect(x, yPos, wAdr, headerHeight),
                         Qt::AlignCenter, "Adresse");
        x += wAdr;
        painter.drawText(QRect(x, yPos, wNom, headerHeight),
                         Qt::AlignCenter, "Propriétaire");
        x += wNom;
        painter.drawText(QRect(x, yPos, wNum, headerHeight),
                         Qt::AlignCenter, "Numéro");
        x += wNum;
        painter.drawText(QRect(x, yPos, wPrix, headerHeight),
                         Qt::AlignCenter, "Prix");

        yPos += headerHeight + 5;
        painter.setFont(font);
    };

    drawHeaderRow(y);

    const int baseRowHeight = 26;
    int count = 0;

    while (q.next()) {
        if (y > pageBottom - 4 * baseRowHeight) {
            pdf.newPage();
            y = topMargin;
            drawHeaderRow(y);
        }

        const QString id     = q.value(0).toString();
        const QString type   = q.value(1).toString();
        const QString ville  = q.value(2).toString();
        const QString adresse= q.value(3).toString();
        const QString nom    = q.value(4).toString();
        const QString numero = q.value(5).toString();
        const double prix    = q.value(6).toDouble();
        const QString prixTxt = QLocale().toString(prix, 'f', 2);

        QRect rId   = fm.boundingRect(0, 0, wId,   1000, Qt::TextWordWrap, id);
        QRect rType = fm.boundingRect(0, 0, wType, 1000, Qt::TextWordWrap, type);
        QRect rVille= fm.boundingRect(0, 0, wVille,1000, Qt::TextWordWrap, ville);
        QRect rAdr  = fm.boundingRect(0, 0, wAdr,  1000, Qt::TextWordWrap, adresse);
        QRect rNom  = fm.boundingRect(0, 0, wNom,  1000, Qt::TextWordWrap, nom);
        QRect rNum  = fm.boundingRect(0, 0, wNum,  1000, Qt::TextWordWrap, numero);
        QRect rPrix = fm.boundingRect(0, 0, wPrix, 1000, Qt::TextWordWrap, prixTxt);

        int rowHeight = baseRowHeight;
        rowHeight = qMax(rowHeight, rId.height());
        rowHeight = qMax(rowHeight, rType.height());
        rowHeight = qMax(rowHeight, rVille.height());
        rowHeight = qMax(rowHeight, rAdr.height());
        rowHeight = qMax(rowHeight, rNom.height());
        rowHeight = qMax(rowHeight, rNum.height());
        rowHeight = qMax(rowHeight, rPrix.height());
        rowHeight += 8;

        int x = margin;

        painter.setPen(Qt::black);
        painter.setBrush(Qt::NoBrush);

        painter.drawRect(x, y, wId, rowHeight);
        painter.drawRect(x + wId, y, wType, rowHeight);
        painter.drawRect(x + wId + wType, y, wVille, rowHeight);
        painter.drawRect(x + wId + wType + wVille, y, wAdr, rowHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr, y, wNom, rowHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr + wNom, y, wNum, rowHeight);
        painter.drawRect(x + wId + wType + wVille + wAdr + wNom + wNum, y, wPrix, rowHeight);

        x = margin;

        painter.drawText(QRect(x, y, wId, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, id);
        x += wId;

        painter.drawText(QRect(x, y, wType, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, type);
        x += wType;

        painter.drawText(QRect(x, y, wVille, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, ville);
        x += wVille;

        painter.drawText(QRect(x, y, wAdr, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, adresse);
        x += wAdr;

        painter.drawText(QRect(x, y, wNom, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, nom);
        x += wNom;

        painter.drawText(QRect(x, y, wNum, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, numero);
        x += wNum;

        painter.drawText(QRect(x, y, wPrix, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, prixTxt);

        y += rowHeight;
        ++count;
    }

    painter.end();

    if (count == 0) {
        QMessageBox::information(
            this,
            "PDF",
            "Aucun local non disponible trouvé.\n"
            "PDF généré (vide) dans:\n" + filePath
            );
    } else {
        QMessageBox::information(
            this,
            "PDF",
            QString("PDF généré dans:\n%1\n%2 local(aux) listé(s).")
                .arg(filePath)
                .arg(count)
            );
    }
}

/* ---- Historique des services (dernier mois) ---- */
void MainWindow::on_pb_historiqueService_clicked()
{
    const QString baseDirPath = QStringLiteral("C:/Users/firas/Desktop/local de pdf qt");
    QDir dir(baseDirPath);
    const QString path = dir.filePath("historique_local.txt");

    QFile file(path);
    if (!file.exists()) {
        QMessageBox::information(this, "Historique",
                                 "Aucune action enregistrée pour les locaux.");
        if (m_histModel)
            m_histModel->removeRows(0, m_histModel->rowCount());
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur",
                             "Impossible d'ouvrir le fichier historique.");
        return;
    }

    QTextStream in(&file);
    const QString contenu = in.readAll();
    file.close();

    const QStringList lignes = contenu.split('\n', Qt::SkipEmptyParts);
    QDateTime ilyaUnMois = QDateTime::currentDateTime().addMonths(-1);

    if (!m_histModel) {
        QMessageBox::critical(this, "UI",
                              "Modèle d'historique (m_histModel) non initialisé.");
        return;
    }

    // vider le tableau avant de le remplir
    m_histModel->removeRows(0, m_histModel->rowCount());

    int row = 0;
    for (const QString &ligne : lignes) {
        // la date/heure est avant le premier '|'
        const QString dateString = ligne.section('|', 0, 0).trimmed();
        QDateTime date = QDateTime::fromString(dateString, "dd/MM/yyyy hh:mm:ss");
        if (!date.isValid())
            continue;

        if (date < ilyaUnMois)
            continue;

        // le reste de la ligne = description de l'action
        QString action = ligne.section('|', 1).trimmed(); // tout après le premier '|'
        if (action.isEmpty())
            action = "(action vide)";

        m_histModel->insertRow(row);
        m_histModel->setData(m_histModel->index(row, 0), date.toString("dd/MM/yyyy hh:mm:ss"));
        m_histModel->setData(m_histModel->index(row, 1), action);
        ++row;
    }

    if (row == 0) {
        QMessageBox::information(this, "Historique",
                                 "Aucune modification durant le dernier mois.");
    }

    if (ui->tableView_2) {
        ui->tableView_2->resizeColumnsToContents();
    }
}


/* ---- Ajouter Réclamation ---- */
void MainWindow::on_pushButton_13_clicked()
{
    auto *leCode  = le(this, "lineEdit_12");
    auto *leDate  = le(this, "lineEdit_2");
    auto *leNom   = le(this, "lineEdit_3");
    auto *leDesc  = le(this, "lineEdit_4");
    auto *leEtat  = le(this, "lineEdit_5");
    auto *leDelai = le(this, "lineEdit_6");

    if (!leCode || !leDate || !leNom || !leDesc || !leEtat || !leDelai) {
        QMessageBox::critical(this, "UI", "Certains QLineEdit sont introuvables.");
        return;
    }

    bool okInt = false;
    const int code = leCode->text().trimmed().toInt(&okInt);
    if (!okInt || code <= 0) {
        QMessageBox::warning(this, "Validation", "CODE doit être un entier > 0.");
        return;
    }

    QDate d;
    if (!parseDateReclamation(leDate->text(), d)) {
        QMessageBox::warning(this, "Validation", "Date invalide. Ex: 31/12/2025, 2025-12-31, 31-12-2025.");
        return;
    }

    const QString nom  = leNom->text().trimmed();
    const QString desc = leDesc->text().trimmed();
    const QString etat = canonEtat(leEtat->text());
    const QString delai= leDelai->text().trimmed();

    Reclamation r(code, d, nom, desc, etat, delai);

    QString err;
    if (!r.isValid(&err)) {
        QMessageBox::warning(this, "Champs invalides", err);
        return;
    }
    if (!r.insert(&err)) {
        if (err.contains("ORA-02290")) err = "ETAT invalide. Valeurs autorisées: OUVERTE, EN_COURS, CLOSE.";
        if (err.contains("ORA-00001")) err = "CODE existe déjà (clé unique).";
        if (err.contains("ORA-04098")) err = "Trigger invalide sur RECLAMATION. Corrige BI_RECLAMATION côté Oracle.";
        QMessageBox::critical(this, "Insertion", err);
        return;
    }

    QMessageBox::information(this, "Succès", "Réclamation ajoutée.");
    leCode->clear();
    leNom->clear();
    leDesc->clear();
    leEtat->clear();
    leDelai->clear();

    loadReclamationsIntoTable();
}

/* ---- Affichage table RECLAMATION ---- */
void MainWindow::loadReclamationsIntoTable()
{
    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");
    const QString sql = QStringLiteral(
                            "SELECT "
                            "  CODE, "
                            "  TO_CHAR(DATE_RECLAMATION, 'YYYY-MM-DD') AS DATE_RECLAMATION, "
                            "  NOM_RESIDENT, "
                            "  DESCRIPTION, "
                            "  ETAT, "
                            "  DELAI "
                            "FROM %1 "
                            "ORDER BY CODE DESC"
                            ).arg(tableName);

    QSqlQuery q(Connection::db());
    if (!q.exec(sql)) {
        const QString e = q.lastError().text();
        qWarning() << "[RECLAMATION] SELECT failed:" << e;
        QMessageBox::critical(this, "Erreur", "Lecture réclamations: " + e);
        return;
    }

    QTableWidget* tw = ui->tableWidget;
    if (!tw) {
        QMessageBox::critical(this, "UI", "QTableWidget 'tableWidget' introuvable.");
        return;
    }

    tw->clear();
    tw->setColumnCount(6);
    tw->setHorizontalHeaderLabels(QStringList()
                                  << "Code" << "Date" << "Nom résident"
                                  << "Description" << "État" << "Délai");
    tw->setRowCount(0);
    tw->setAlternatingRowColors(true);
    tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->horizontalHeader()->setStretchLastSection(true);

    int row = 0;
    while (q.next()) {
        tw->insertRow(row);

        const int    code  = q.value(0).toInt();
        const QString date = q.value(1).toString();
        const QString nom  = q.value(2).toString();
        const QString desc = q.value(3).toString();
        const QString etat = q.value(4).toString();
        const QString delai= q.value(5).toString();

        QString descDisplay = desc;
        if (descDisplay.size() > 120) descDisplay = descDisplay.left(120) + "…";

        auto mk = [](const QVariant& v) {
            auto *it = new QTableWidgetItem;
            it->setData(Qt::DisplayRole, v);
            it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            return it;
        };

        auto *itCode = mk(code);
        itCode->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        tw->setItem(row, 0, itCode);
        tw->setItem(row, 1, mk(date));
        tw->setItem(row, 2, mk(nom));

        auto *itDesc = mk(descDisplay);
        if (!desc.isEmpty()) itDesc->setToolTip(desc);
        tw->setItem(row, 3, itDesc);

        tw->setItem(row, 4, mk(etat));
        tw->setItem(row, 5, mk(delai));

        ++row;
    }

    if (ui->comboBox_2) {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({"OUVERTE","EN_COURS","CLOSE"});
    }

    if (ui->tableWidget) {
        connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
                this, &MainWindow::syncReclamSelectionToForm);
    }

    tw->resizeColumnsToContents();
}

/* ---- Supprimer Réclamation ---- */
void MainWindow::on_pushButton_12_clicked()
{
    QTableWidget* tw = ui->tableWidget;
    if (!tw) {
        QMessageBox::critical(this, "UI", "QTableWidget 'tableWidget' introuvable.");
        return;
    }

    const auto sel = tw->selectionModel()
                         ? tw->selectionModel()->selectedRows()
                         : QModelIndexList{};
    if (sel.isEmpty()) {
        QMessageBox::information(this, "Suppression", "Sélectionne une ligne d'abord.");
        return;
    }

    QVector<int> rows;
    rows.reserve(sel.size());
    QVector<int> codes;
    codes.reserve(sel.size());

    for (const QModelIndex& mi : sel) {
        const int row = mi.row();
        rows.push_back(row);

        QTableWidgetItem* itCode = tw->item(row, 0);
        bool ok = false;
        const int code = itCode ? itCode->text().toInt(&ok) : 0;
        if (!ok || code <= 0) {
            QMessageBox::warning(this, "Suppression", "CODE invalide dans la ligne sélectionnée.");
            return;
        }
        codes.push_back(code);
    }

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    if (!db.transaction()) {
        QMessageBox::critical(this, "DB", "Impossible de démarrer la transaction.");
        return;
    }

    QSqlQuery del(db);
    del.prepare(QStringLiteral("DELETE FROM %1 WHERE CODE = :code").arg(tableName));

    for (int code : codes) {
        del.bindValue(":code", code);
        if (!del.exec()) {
            const QString err = del.lastError().text();
            db.rollback();
            QMessageBox::critical(this, "Suppression",
                                  "Échec suppression (CODE=" + QString::number(code) + "): " + err);
            return;
        }
    }

    if (!db.commit()) {
        QMessageBox::critical(this, "DB", "Commit échoué.");
        return;
    }

    std::sort(rows.begin(), rows.end(), std::greater<int>());
    for (int r : rows)
        tw->removeRow(r);
}

/* ---- Synchro sélection réclamation -> formulaire ---- */
void MainWindow::syncReclamSelectionToForm()
{
    QTableWidget* tw = ui->tableWidget;
    if (!tw) return;

    const auto sel = tw->selectionModel()
                         ? tw->selectionModel()->selectedRows()
                         : QModelIndexList{};
    if (sel.isEmpty()) return;

    const int row = sel.first().row();

    const QString code  = tw->item(row, 0) ? tw->item(row, 0)->text().trimmed() : QString();
    const QString date  = tw->item(row, 1) ? tw->item(row, 1)->text().trimmed() : QString();
    const QString nom   = tw->item(row, 2) ? tw->item(row, 2)->text().trimmed() : QString();
    const QString desc  = tw->item(row, 3)
                             ? (tw->item(row, 3)->toolTip().isEmpty()
                                    ? tw->item(row, 3)->text().trimmed()
                                    : tw->item(row, 3)->toolTip())
                             : QString();
    const QString etat  = tw->item(row, 4) ? tw->item(row, 4)->text().trimmed() : QString();
    const QString delai = tw->item(row, 5) ? tw->item(row, 5)->text().trimmed() : QString();

    if (auto *le = this->findChild<QLineEdit*>("lineEdit_13")) le->setText(code);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_14")) le->setText(date);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_15")) le->setText(nom);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_16")) le->setText(desc);
    if (ui->comboBox_2) {
        int idx = ui->comboBox_2->findText(etat, Qt::MatchFixedString);
        if (idx < 0) idx = 0;
        ui->comboBox_2->setCurrentIndex(idx);
    }
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_17")) le->setText(delai);
}

/* ---- Modifier Réclamation ---- */
void MainWindow::on_pushButton_17_clicked()
{
    auto *leCode  = this->findChild<QLineEdit*>("lineEdit_13");
    auto *leDate  = this->findChild<QLineEdit*>("lineEdit_14");
    auto *leNom   = this->findChild<QLineEdit*>("lineEdit_15");
    auto *leDesc  = this->findChild<QLineEdit*>("lineEdit_16");
    auto *leDelai = this->findChild<QLineEdit*>("lineEdit_17");
    auto *cbEtat  = ui->comboBox_2;

    if (!leCode || !leDate || !leNom || !leDesc || !leDelai || !cbEtat) {
        QMessageBox::critical(this, "UI", "Champs d'édition introuvables.");
        return;
    }

    bool ok = false;
    const int code = leCode->text().trimmed().toInt(&ok);
    if (!ok || code <= 0) {
        QMessageBox::warning(this, "Validation", "CODE invalide.");
        return;
    }

    QDate d;
    if (!parseDateReclamation(leDate->text(), d)) {
        QMessageBox::warning(this, "Validation", "DATE invalide. Ex: 2025-12-31 ou 31/12/2025.");
        return;
    }

    const QString nom   = leNom->text().trimmed();
    const QString desc  = leDesc->text().trimmed();
    const QString etat  = cbEtat->currentText().trimmed();
    const QString delai = leDelai->text().trimmed();

    if (nom.isEmpty())  {
        QMessageBox::warning(this, "Validation", "NOM_RESIDENT est obligatoire.");
        return;
    }
    if (desc.isEmpty()) {
        QMessageBox::warning(this, "Validation", "DESCRIPTION est obligatoire.");
        return;
    }

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");
    QSqlQuery q(db);
    q.prepare(QStringLiteral(
                  "UPDATE %1 SET "
                  "  DATE_RECLAMATION = :d, "
                  "  NOM_RESIDENT     = :n, "
                  "  DESCRIPTION      = :x, "
                  "  ETAT             = :e, "
                  "  DELAI            = :l "
                  "WHERE CODE = :c"
                  ).arg(tableName));

    q.bindValue(":d", d.startOfDay());
    q.bindValue(":n", nom);
    q.bindValue(":x", desc);
    q.bindValue(":e", etat);
    q.bindValue(":l", delai.isEmpty() ? QVariant() : QVariant(delai));
    q.bindValue(":c", code);

    if (!q.exec()) {
        const QString err = q.lastError().text();
        QMessageBox::critical(this, "Modification", "Échec UPDATE: " + err);
        return;
    }
    if (q.numRowsAffected() <= 0) {
        QMessageBox::warning(this, "Modification", "Aucune ligne mise à jour. CODE introuvable ?");
        return;
    }

    loadReclamationsIntoTable();

    if (ui->tableWidget) {
        for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
            auto *it = ui->tableWidget->item(r, 0);
            if (it && it->text().toInt() == code) {
                ui->tableWidget->setCurrentCell(r, 0);
                break;
            }
        }
    }

    QMessageBox::information(this, "Succès", "Réclamation modifiée.");
}

/* ---- Statistiques Local (camembert dispo / non dispo) ---- */
void MainWindow::on_statistiqueButton_2_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    int nbDispo = 0;
    int nbNonDispo = 0;

    QSqlQuery q(db);
    if (!q.exec("SELECT DISPONIBILITE, COUNT(*) "
                "FROM LOCAL "
                "GROUP BY DISPONIBILITE")) {
        QMessageBox::critical(this, "Statistiques",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    while (q.next()) {
        const int dispo = q.value(0).toInt();
        const int count = q.value(1).toInt();
        if (dispo == 1)
            nbDispo = count;
        else if (dispo == 0)
            nbNonDispo = count;
    }

    if (nbDispo == 0 && nbNonDispo == 0) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucun local trouvé dans la base.");
        return;
    }

    QPieSeries *series = new QPieSeries();
    series->append("Disponibles",     nbDispo);
    series->append("Non disponibles", nbNonDispo);

    if (series->slices().size() >= 2) {
        QPieSlice *sliceDispo = series->slices().at(0);
        QPieSlice *sliceNonDispo = series->slices().at(1);

        sliceDispo->setBrush(QColor("#5cb5f2"));
        sliceNonDispo->setBrush(QColor("#435AD9"));

        sliceDispo->setLabel(QString("Disponibles (%1)").arg(nbDispo));
        sliceNonDispo->setLabel(QString("Non disponibles (%1)").arg(nbNonDispo));
    }

    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des locaux (disponibles / non disponibles)");
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->verticalLayout) {
        QLayoutItem *item;
        while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }
        ui->verticalLayout->addWidget(chartView);
    } else {
        QMessageBox::critical(this, "UI", "QVBoxLayout 'verticalLayout' introuvable.");
        chart->deleteLater();
    }
}

/* ---- Statistiques simple par ville ---- */
void MainWindow::on_statistiqueButton_clicked()
{
    if (!ui->listWidget) {
        QMessageBox::critical(this, "UI", "QListWidget 'listWidget' introuvable.");
        return;
    }

    ui->listWidget->clear();

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    QSqlQuery q(db);
    if (!q.exec("SELECT VILLE, COUNT(*) FROM LOCAL GROUP BY VILLE ORDER BY VILLE")) {
        ui->listWidget->addItem("Erreur SQL: " + q.lastError().text());
        return;
    }

    while (q.next()) {
        const QString ville = q.value(0).toString();
        const int count     = q.value(1).toInt();
        ui->listWidget->addItem(QString("%1 : %2 locaux").arg(ville).arg(count));
    }

    if (ui->listWidget->count() == 0) {
        ui->listWidget->addItem("Aucune ville trouvée dans LOCAL.");
    }
}
void MainWindow::on_statistiqueButton_3_clicked()
{
    // 1) Vérifier la base
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    // 2) Récupérer les données : nombre de locaux par TYPE_LOCAL
    QSqlQuery q(db);
    if (!q.exec("SELECT TYPE_LOCAL, COUNT(*) "
                "FROM LOCAL "
                "GROUP BY TYPE_LOCAL "
                "ORDER BY TYPE_LOCAL")) {
        QMessageBox::critical(this, "Statistiques",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    QStringList types;
    QVector<int> counts;
    int maxCount = 0;

    while (q.next()) {
        const QString type = q.value(0).toString();
        const int nb       = q.value(1).toInt();
        types << type;
        counts << nb;
        if (nb > maxCount) maxCount = nb;
    }

    if (types.isEmpty()) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucun local trouvé dans la base.");
        return;
    }

    // 3) Construire la série lissée (courbe) : x = index, y = count
    QSplineSeries *series = new QSplineSeries();
    series->setName("Nombre de locaux par type");

    for (int i = 0; i < counts.size(); ++i) {
        series->append(i, counts[i]);   // X = index, Y = nombre de locaux
    }

    // 4) Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des locaux par type (courbe)");

    // 5) Axe X : catégories (noms des types)
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("Type de local");

    for (int i = 0; i < types.size(); ++i) {
        axisX->append(types[i], i);   // étiquette 'TYPE_LOCAL' à la position i
    }
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    // 6) Axe Y : nombre de locaux
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de locaux");
    axisY->setLabelFormat("%d");
    axisY->setRange(0, maxCount + 1);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // 7) ChartView
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 8) Mettre le graphe dans verticalLayout_2
    if (ui->verticalLayout_2) {
        QLayoutItem *item;
        while ((item = ui->verticalLayout_2->takeAt(0)) != nullptr) {
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }
        ui->verticalLayout_2->addWidget(chartView);
    } else {
        QMessageBox::critical(this, "UI", "QVBoxLayout 'verticalLayout_2' introuvable.");
        chart->deleteLater();
    }
}
// les metiers de azyz//

void MainWindow::on_pushButton_8_clicked()
{
    // QLineEdit où tu écris le nom du résident
    QLineEdit *leNom = this->findChild<QLineEdit*>("lineEdit");
    if (!leNom) {
        QMessageBox::critical(this, "UI",
                              "QLineEdit 'lineEdit' introuvable (recherche NOM_RESIDENT).");
        return;
    }

    const QString texte = leNom->text().trimmed();

    // Si vide -> on réaffiche toutes les réclamations
    if (texte.isEmpty()) {
        loadReclamationsIntoTable();
        return;
    }

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    // Requête filtrée sur NOM_RESIDENT (insensible à la casse)
    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    QString sql =
        QStringLiteral(
            "SELECT "
            "  CODE, "
            "  TO_CHAR(DATE_RECLAMATION, 'YYYY-MM-DD') AS DATE_RECLAMATION, "
            "  NOM_RESIDENT, "
            "  DESCRIPTION, "
            "  ETAT, "
            "  DELAI "
            "FROM %1 "
            "WHERE UPPER(NOM_RESIDENT) LIKE :nom "
            "ORDER BY CODE DESC"
            ).arg(tableName);

    QSqlQuery q(db);
    if (!q.prepare(sql)) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Préparation SELECT échouée: " + q.lastError().text());
        return;
    }

    QString pattern = texte.toUpper();
    pattern.replace("'", "''"); // sécuriser les quotes
    q.bindValue(":nom", "%" + pattern + "%");

    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "SELECT échoué: " + q.lastError().text());
        return;
    }

    // On remplit tableWidget comme dans loadReclamationsIntoTable()
    QTableWidget* tw = ui->tableWidget;
    if (!tw) {
        QMessageBox::critical(this, "UI", "QTableWidget 'tableWidget' introuvable.");
        return;
    }

    tw->clear();
    tw->setColumnCount(6);
    tw->setHorizontalHeaderLabels(QStringList()
                                  << "Code" << "Date" << "Nom résident"
                                  << "Description" << "État" << "Délai");
    tw->setRowCount(0);
    tw->setAlternatingRowColors(true);
    tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->horizontalHeader()->setStretchLastSection(true);

    int row = 0;
    while (q.next()) {
        tw->insertRow(row);

        const int    code  = q.value(0).toInt();
        const QString date = q.value(1).toString();
        const QString nom  = q.value(2).toString();
        const QString desc = q.value(3).toString();
        const QString etat = q.value(4).toString();
        const QString delai= q.value(5).toString();

        QString descDisplay = desc;
        if (descDisplay.size() > 120) descDisplay = descDisplay.left(120) + "…";

        auto mk = [](const QVariant& v) {
            auto *it = new QTableWidgetItem;
            it->setData(Qt::DisplayRole, v);
            it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            return it;
        };

        auto *itCode = mk(code);
        itCode->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        tw->setItem(row, 0, itCode);
        tw->setItem(row, 1, mk(date));
        tw->setItem(row, 2, mk(nom));

        auto *itDesc = mk(descDisplay);
        if (!desc.isEmpty()) itDesc->setToolTip(desc);
        tw->setItem(row, 3, itDesc);

        tw->setItem(row, 4, mk(etat));
        tw->setItem(row, 5, mk(delai));

        ++row;
    }

    tw->resizeColumnsToContents();

    if (row == 0) {
        QMessageBox::information(this, "Recherche",
                                 "Aucune réclamation trouvée pour ce nom de résident.");
    }
}
void MainWindow::on_pushButton_9_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    // Tri par DATE_RECLAMATION (de l'ancienne à la plus récente)
    const QString sql = QStringLiteral(
                            "SELECT "
                            "  CODE, "
                            "  TO_CHAR(DATE_RECLAMATION, 'YYYY-MM-DD') AS DATE_RECLAMATION, "
                            "  NOM_RESIDENT, "
                            "  DESCRIPTION, "
                            "  ETAT, "
                            "  DELAI "
                            "FROM %1 "
                            "ORDER BY DATE_RECLAMATION ASC, CODE ASC"
                            ).arg(tableName);

    QSqlQuery q(db);
    if (!q.exec(sql)) {
        const QString e = q.lastError().text();
        QMessageBox::critical(this, "Erreur", "Lecture réclamations (tri date): " + e);
        return;
    }

    QTableWidget* tw = ui->tableWidget;
    if (!tw) {
        QMessageBox::critical(this, "UI", "QTableWidget 'tableWidget' introuvable.");
        return;
    }

    tw->clear();
    tw->setColumnCount(6);
    tw->setHorizontalHeaderLabels(QStringList()
                                  << "Code" << "Date" << "Nom résident"
                                  << "Description" << "État" << "Délai");
    tw->setRowCount(0);
    tw->setAlternatingRowColors(true);
    tw->setSelectionBehavior(QAbstractItemView::SelectRows);
    tw->setSelectionMode(QAbstractItemView::SingleSelection);
    tw->horizontalHeader()->setStretchLastSection(true);

    int row = 0;
    while (q.next()) {
        tw->insertRow(row);

        const int    code  = q.value(0).toInt();
        const QString date = q.value(1).toString();
        const QString nom  = q.value(2).toString();
        const QString desc = q.value(3).toString();
        const QString etat = q.value(4).toString();
        const QString delai= q.value(5).toString();

        QString descDisplay = desc;
        if (descDisplay.size() > 120) descDisplay = descDisplay.left(120) + "…";

        auto mk = [](const QVariant& v) {
            auto *it = new QTableWidgetItem;
            it->setData(Qt::DisplayRole, v);
            it->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            return it;
        };

        auto *itCode = mk(code);
        itCode->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        tw->setItem(row, 0, itCode);
        tw->setItem(row, 1, mk(date));
        tw->setItem(row, 2, mk(nom));

        auto *itDesc = mk(descDisplay);
        if (!desc.isEmpty()) itDesc->setToolTip(desc);
        tw->setItem(row, 3, itDesc);

        tw->setItem(row, 4, mk(etat));
        tw->setItem(row, 5, mk(delai));

        ++row;
    }

    tw->resizeColumnsToContents();

    if (row == 0) {
        QMessageBox::information(this, "Tri par date",
                                 "Aucune réclamation trouvée.");
    }
}
void MainWindow::on_statistiqueButton_10_clicked()
{
    qDebug() << "Slot on_statistiqueButton_10_clicked appelé";
    QMessageBox::information(this, "DEBUG", "Bouton statistiqueButton_10 cliqué");


    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    QSqlQuery q(db);
    if (!q.exec(QStringLiteral(
                    "SELECT NOM_RESIDENT, COUNT(*) "
                    "FROM %1 "
                    "GROUP BY NOM_RESIDENT "
                    "ORDER BY NOM_RESIDENT"
                    ).arg(tableName))) {
        QMessageBox::critical(this, "Statistiques",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    QStringList noms;
    QVector<int> counts;
    int maxCount = 0;

    while (q.next()) {
        const QString nom = q.value(0).toString();
        const int nb      = q.value(1).toInt();

        noms << nom;
        counts << nb;
        if (nb > maxCount) maxCount = nb;
    }

    qDebug() << "Nombre de résidents en stats =" << noms.size();

    if (noms.isEmpty()) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucune réclamation trouvée dans la base.");
        return;
    }

    QSplineSeries *series = new QSplineSeries();
    series->setName("Réclamations par résident");

    for (int i = 0; i < counts.size(); ++i) {
        series->append(i, counts[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre de réclamations par résident");

    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("Nom du résident");

    for (int i = 0; i < noms.size(); ++i) {
        axisX->append(noms[i], i);
    }
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de réclamations");
    axisY->setLabelFormat("%d");
    axisY->setRange(0, maxCount + 1);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->verticalLayout_7) {
        QLayoutItem *item;
        while ((item = ui->verticalLayout_7->takeAt(0)) != nullptr) {
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }
        ui->verticalLayout_7->addWidget(chartView);
    } else {
        QMessageBox::critical(this, "UI", "QVBoxLayout 'verticalLayout_7' introuvable.");
        chart->deleteLater();
    }
}
void MainWindow::on_statistiqueButton_11_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    // On tronque la DESCRIPTION CLOB à 200 caractères pour pouvoir faire un GROUP BY
    const QString sql = QStringLiteral(
                            "SELECT "
                            "  DBMS_LOB.SUBSTR(DESCRIPTION, 200, 1) AS DESC_TXT, "
                            "  COUNT(*) "
                            "FROM %1 "
                            "GROUP BY DBMS_LOB.SUBSTR(DESCRIPTION, 200, 1) "
                            "ORDER BY DESC_TXT"
                            ).arg(tableName);

    QSqlQuery q(db);
    if (!q.exec(sql)) {
        QMessageBox::critical(this, "Statistiques",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    QStringList descriptions;
    QVector<int> counts;
    int maxCount = 0;

    while (q.next()) {
        const QString desc = q.value(0).toString();   // DESC_TXT
        const int nb       = q.value(1).toInt();

        descriptions << desc;
        counts << nb;
        if (nb > maxCount) maxCount = nb;
    }

    if (descriptions.isEmpty()) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucune réclamation trouvée dans la base.");
        return;
    }

    // Courbe lissée : x = index de la description, y = nombre de réclamations
    QSplineSeries *series = new QSplineSeries();
    series->setName("Réclamations par description");

    for (int i = 0; i < counts.size(); ++i) {
        series->append(i, counts[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Nombre de réclamations par description");

    // Axe X : descriptions (on coupe les labels trop longs)
    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("Description");

    for (int i = 0; i < descriptions.size(); ++i) {
        QString label = descriptions[i];
        if (label.size() > 25)
            label = label.left(25) + "...";
        axisX->append(label, i);
    }
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    // Axe Y : nombre de réclamations
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de réclamations");
    axisY->setLabelFormat("%d");
    axisY->setRange(0, maxCount + 1);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Affichage dans verticalLayout_11
    if (ui->verticalLayout_11) {
        QLayoutItem *item;
        while ((item = ui->verticalLayout_11->takeAt(0)) != nullptr) {
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }
        ui->verticalLayout_11->addWidget(chartView);
    } else {
        QMessageBox::critical(this, "UI",
                              "QVBoxLayout 'verticalLayout_11' introuvable.");
        chart->deleteLater();
    }
}
void MainWindow::on_pushButton_10_clicked()
{
    // 1) Récupérer l'état choisi dans comboBox_2
    if (!ui->comboBox_2) {
        QMessageBox::critical(this, "UI", "comboBox_2 (état) introuvable.");
        return;
    }

    const QString etat = ui->comboBox_2->currentText().trimmed();
    if (etat.isEmpty()) {
        QMessageBox::warning(this, "État manquant",
                             "Choisis un état dans la liste (OUVERTE, EN_COURS, CLOSE).");
        return;
    }

    // 2) Connexion base
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    // 3) Récupérer les réclamations pour cet état
    //    On tronque DESCRIPTION (CLOB) à 200 caractères
    QSqlQuery q(db);
    q.prepare(QStringLiteral(
                  "SELECT "
                  "  CODE, "
                  "  TO_CHAR(DATE_RECLAMATION, 'YYYY-MM-DD') AS DATE_RECLAMATION, "
                  "  NOM_RESIDENT, "
                  "  DBMS_LOB.SUBSTR(DESCRIPTION, 200, 1) AS DESC_TXT, "
                  "  ETAT, "
                  "  DELAI "
                  "FROM %1 "
                  "WHERE ETAT = :e "
                  "ORDER BY DATE_RECLAMATION ASC, CODE ASC"
                  ).arg(tableName));
    q.bindValue(":e", etat);

    if (!q.exec()) {
        QMessageBox::critical(this, "PDF",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    QList<QVector<QString>> rows;
    while (q.next()) {
        QVector<QString> r(6);
        r[0] = q.value(0).toString(); // CODE
        r[1] = q.value(1).toString(); // DATE
        r[2] = q.value(2).toString(); // NOM_RESIDENT
        r[3] = q.value(3).toString(); // DESCRIPTION tronquée
        r[4] = q.value(4).toString(); // ETAT
        r[5] = q.value(5).toString(); // DELAI
        rows.push_back(r);
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, "PDF",
                                 "Aucune réclamation trouvée pour l'état " + etat + ".");
        return;
    }

    // 4) Dossier de sortie
    const QString baseDirPath = QStringLiteral("C:/Users/firas/Desktop/local de pdf qt");
    QDir baseDir(baseDirPath);
    if (!baseDir.exists()) {
        if (!QDir().mkpath(baseDirPath)) {
            QMessageBox::critical(this, "PDF",
                                  "Impossible de créer le dossier:\n" + baseDirPath);
            return;
        }
    }

    QString safeEtat = etat;
    safeEtat.replace(' ', '_');
    safeEtat.replace('/', '_');

    const QString filePath =
        baseDir.filePath(QString("reclamations_%1.pdf").arg(safeEtat));

    // 5) Création du PDF
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "PDF", "Impossible de créer le PDF.");
        return;
    }

    QFont font("Helvetica", 10);
    painter.setFont(font);
    QFontMetrics fm(font);

    const int margin      = 60;
    const int topMargin   = 80;
    const int usableWidth = pdf.width() - 2 * margin;
    const int pageBottom  = pdf.height() - margin;

    int y = topMargin;

    // Titre
    QFont titleFont("Helvetica", 16, QFont::Bold);
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, usableWidth, 30),
                     Qt::AlignCenter,
                     QString("Réclamations - état %1").arg(etat));
    y += 45;

    painter.setFont(font);
    const QString dateStr =
        QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    painter.drawText(QRect(margin, y, usableWidth, 20),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "Généré le: " + dateStr);
    y += 35;

    // Largeurs de colonnes
    const int wCode = int(usableWidth * 0.08);
    const int wDate = int(usableWidth * 0.12);
    const int wNom  = int(usableWidth * 0.18);
    const int wDesc = int(usableWidth * 0.40);
    const int wEtat = int(usableWidth * 0.10);
    const int wDelai= usableWidth - (wCode + wDate + wNom + wDesc + wEtat);

    // --- entête de tableau, avec plus de hauteur ---
    auto drawHeaderRow = [&](int& yPos) {
        QFont headerFont("Helvetica", 11, QFont::Bold);
        painter.setFont(headerFont);

        int x = margin;
        const int headerHeight = 32;   // plus haut qu'avant

        painter.drawRect(x, yPos, wCode, headerHeight);
        painter.drawRect(x + wCode, yPos, wDate, headerHeight);
        painter.drawRect(x + wCode + wDate, yPos, wNom, headerHeight);
        painter.drawRect(x + wCode + wDate + wNom, yPos, wDesc, headerHeight);
        painter.drawRect(x + wCode + wDate + wNom + wDesc, yPos, wEtat, headerHeight);
        painter.drawRect(x + wCode + wDate + wNom + wDesc + wEtat, yPos, wDelai, headerHeight);

        painter.drawText(QRect(x, yPos, wCode, headerHeight),
                         Qt::AlignCenter, "Code");
        x += wCode;
        painter.drawText(QRect(x, yPos, wDate, headerHeight),
                         Qt::AlignCenter, "Date");
        x += wDate;
        painter.drawText(QRect(x, yPos, wNom, headerHeight),
                         Qt::AlignCenter, "Nom");
        x += wNom;
        painter.drawText(QRect(x, yPos, wDesc, headerHeight),
                         Qt::AlignCenter, "Description");
        x += wDesc;
        painter.drawText(QRect(x, yPos, wEtat, headerHeight),
                         Qt::AlignCenter, "État");
        x += wEtat;
        painter.drawText(QRect(x, yPos, wDelai, headerHeight),
                         Qt::AlignCenter, "Délai");

        yPos += headerHeight + 10;   // plus d'espace après l'entête
        painter.setFont(font);
    };

    drawHeaderRow(y);

    // hauteur de base plus grande pour aérer
    const int baseRowHeight = 30;

    for (const auto& r : rows) {
        if (y > pageBottom - 5 * baseRowHeight) {
            pdf.newPage();
            y = topMargin;
            drawHeaderRow(y);
        }

        const QString code = r[0];
        const QString date = r[1];
        const QString nom  = r[2];
        const QString desc = r[3];
        const QString et   = r[4];
        const QString del  = r[5];

        QRect rCode = fm.boundingRect(0, 0, wCode,  1000, Qt::TextWordWrap, code);
        QRect rDate = fm.boundingRect(0, 0, wDate,  1000, Qt::TextWordWrap, date);
        QRect rNom  = fm.boundingRect(0, 0, wNom,   1000, Qt::TextWordWrap, nom);
        QRect rDesc = fm.boundingRect(0, 0, wDesc,  1000, Qt::TextWordWrap, desc);
        QRect rEtat = fm.boundingRect(0, 0, wEtat,  1000, Qt::TextWordWrap, et);
        QRect rDel  = fm.boundingRect(0, 0, wDelai, 1000, Qt::TextWordWrap, del);

        int rowHeight = baseRowHeight;
        rowHeight = qMax(rowHeight, rCode.height());
        rowHeight = qMax(rowHeight, rDate.height());
        rowHeight = qMax(rowHeight, rNom.height());
        rowHeight = qMax(rowHeight, rDesc.height());
        rowHeight = qMax(rowHeight, rEtat.height());
        rowHeight = qMax(rowHeight, rDel.height());
        rowHeight += 12;   // gros padding vertical pour espacer les lignes

        int x = margin;

        painter.drawRect(x, y, wCode, rowHeight);
        painter.drawRect(x + wCode, y, wDate, rowHeight);
        painter.drawRect(x + wCode + wDate, y, wNom, rowHeight);
        painter.drawRect(x + wCode + wDate + wNom, y, wDesc, rowHeight);
        painter.drawRect(x + wCode + wDate + wNom + wDesc, y, wEtat, rowHeight);
        painter.drawRect(x + wCode + wDate + wNom + wDesc + wEtat, y, wDelai, rowHeight);

        x = margin;

        painter.drawText(QRect(x, y, wCode, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, code);
        x += wCode;
        painter.drawText(QRect(x, y, wDate, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, date);
        x += wDate;
        painter.drawText(QRect(x, y, wNom, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, nom);
        x += wNom;
        painter.drawText(QRect(x, y, wDesc, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, desc);
        x += wDesc;
        painter.drawText(QRect(x, y, wEtat, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, et);
        x += wEtat;
        painter.drawText(QRect(x, y, wDelai, rowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, del);

        y += rowHeight;
    }

    painter.end();

    QMessageBox::information(
        this,
        "PDF",
        QString("PDF généré dans:\n%1\n%2 réclamation(s) listée(s).")
            .arg(filePath)
            .arg(rows.size())
        );
}
