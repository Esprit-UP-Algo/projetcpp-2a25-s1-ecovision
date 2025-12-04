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

#include <QStandardItemModel>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMap>


#include "smsmanager.h"
#include "notificationwidget.h"
#include <QTimer>

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QLabel>
#include <QGraphicsDropShadowEffect>

#include <QSystemTrayIcon>
#include <QIcon>



#include <QProcess>
#ifdef Q_OS_WIN
#include <windows.h>
#endif



/* ---- helpers ---- */
static bool parseDateReclamation(const QString& s, QDate& out) {
    const QStringList fmts = {"dd/MM/yyyy", "yyyy-MM-dd", "dd-MM-yyyy"};
    for (const auto& f : fmts) {
        const QDate d = QDate::fromString(s.trimmed(), f);
        if (d.isValid()) { out = d; return true; }
    }
    return false;
}

// normalisation ETAT, avec gestion TRAITE / NON_TRAITE
static QString canonEtat(QString s) {
    s = s.trimmed();
    if (s.isEmpty()) return "OUVERTE";

    s = s.normalized(QString::NormalizationForm_D);
    s.remove(QRegularExpression(u8"[\\p{Mn}]"));
    s.replace(' ', '_');
    s.replace('-', '_');
    s = s.toUpper();

    // NON_TRAITE -> OUVERTE
    if (s == "NON_TRAITE" || s == "NON_TRAITEE" ||
        s == "NON_TRAITES" || s == "NON_TRAITEES" ||
        s == "OUVERT" || s == "OUVERTS" || s == "OUVERTES") {
        return "OUVERTE";
    }

    // EN COURS
    if (s == "ENCOURS" || s == "EN_COUR" || s == "EN_COURS") {
        return "EN_COURS";
    }

    // TRAITE -> CLOSE
    if (s == "TRAITE" || s == "TRAITEE" ||
        s == "TRAITES" || s == "TRAITEES" ||
        s == "CLOS" || s == "CLOTURE" || s == "CLOTUREE" ||
        s == "FERME" || s == "FERMEE") {
        return "CLOSE";
    }

    return s;
}
//ajout de notification de delai passer//



static int delaiToMilliseconds(const QString &delai, bool *okOut = nullptr)
{
    // Format déjà validé par ton QRegularExpressionValidator:
    // "nombre + unité" (jour(s) / heure(s) / seconde(s))
    QRegularExpression rx(
        "^\\s*(\\d+)\\s*(jour|jours|heure|heures|seconde|secondes)\\s*$",
        QRegularExpression::CaseInsensitiveOption
        );
    QRegularExpressionMatch m = rx.match(delai);
    if (!m.hasMatch()) {
        if (okOut) *okOut = false;
        return 0;
    }

    bool ok = false;
    const int value = m.captured(1).toInt(&ok);
    if (!ok || value <= 0) {
        if (okOut) *okOut = false;
        return 0;
    }

    const QString unit = m.captured(2).toLower();
    qint64 ms = 0;

    if (unit == "jour" || unit == "jours") {
        ms = qint64(value) * 24 * 60 * 60 * 1000;
    } else if (unit == "heure" || unit == "heures") {
        ms = qint64(value) * 60 * 60 * 1000;
    } else { // seconde / secondes
        ms = qint64(value) * 1000;
    }

    // QTimer prend un int, on évite le dépassement
    if (ms > std::numeric_limits<int>::max())
        ms = std::numeric_limits<int>::max();

    if (okOut) *okOut = true;
    return int(ms);
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


static qint64 delaiToMs(const QString &delai, bool *okOut = nullptr)
{
    QRegularExpression rx(
        "^\\s*(\\d+)\\s*(jour|jours|heure|heures|seconde|secondes)\\s*$",
        QRegularExpression::CaseInsensitiveOption
        );
    QRegularExpressionMatch m = rx.match(delai);
    if (!m.hasMatch()) {
        if (okOut) *okOut = false;
        return 0;
    }

    int value = m.captured(1).toInt();
    QString unit = m.captured(2).toLower();

    qint64 ms = 0;
    if (unit.startsWith("jour")) {
        ms = qint64(value) * 24 * 60 * 60 * 1000;
    } else if (unit.startsWith("heure")) {
        ms = qint64(value) * 60 * 60 * 1000;
    } else { // seconde(s)
        ms = qint64(value) * 1000;
    }

    if (okOut) *okOut = true;
    return ms;
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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_view(nullptr)
    , m_scene(nullptr)
    , m_marker(nullptr)
    , m_mapItem(nullptr)
    , m_histModel(nullptr)
    , m_smsManager(nullptr)
    , m_trayIcon(nullptr)
{
    ui->setupUi(this);

    /* ==== Icône de notification Windows (systray) ==== */
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_trayIcon = new QSystemTrayIcon(this);
        // Mets un vrai fichier d'icône dans ton .qrc, par ex ":/icons/app.png"
        m_trayIcon->setIcon(QIcon(":/icons/app.png"));
        m_trayIcon->setToolTip("EcoVision - Réclamations");
        m_trayIcon->show();
    }

    /* ==== Initialisation SMSManager ==== */
    m_smsManager = new SMSManager(this);

    connect(m_smsManager, &SMSManager::smsEnvoye,
            this, &MainWindow::onSmsSent);

    connect(m_smsManager, &SMSManager::erreurSMS,
            this, &MainWindow::onSmsError);

    /* ==== Modèle pour l'historique (tableView_2) ==== */
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

    /* ==== Connexion bouton statistiqueButton_10 (si pas auto-connect dans le .ui) ==== */
    if (ui->statistiqueButton_10) {
        connect(ui->statistiqueButton_10,
                &QPushButton::clicked,
                this,
                &MainWindow::on_statistiqueButton_10_clicked);
    }

    /* ==== Schéma Oracle ==== */
    {
        QSqlQuery s(Connection::db());
        s.exec("ALTER SESSION SET CURRENT_SCHEMA=FIRAS");
    }

    /* ==== Validateurs pour LOCAL (ajout) ==== */

    // disponibilite: 0 ou 1
    if (auto *dispoAdd = le(this, "l4")) {
        dispoAdd->setValidator(new QIntValidator(0, 1, this));
    }

    // prix
    if (auto *prixAdd = le(this, "l4_4")) {
        auto *dv = new QDoubleValidator(0.0, 1e12, 2, this);
        dv->setNotation(QDoubleValidator::StandardNotation);
        dv->setLocale(QLocale::system());
        prixAdd->setValidator(dv);
    }

    // CIN_proprietaire : 8 chiffres
    if (auto *cinAdd = le(this, "l4_5")) {
        auto *v = new QRegularExpressionValidator(
            QRegularExpression("^\\d{8}$"),
            this
            );
        cinAdd->setValidator(v);
    }

    // numéro : 8 chiffres
    if (auto *numAdd = le(this, "l4_3")) {
        auto *v = new QRegularExpressionValidator(
            QRegularExpression("^\\d{8}$"),
            this
            );
        numAdd->setValidator(v);
    }

    /* ==== Validateurs pour LOCAL (modification) ==== */

    // ID_LOCAL
    if (auto *idLE = le(this, "lineedit_id_modif")) {
        idLE->setValidator(new QIntValidator(1, INT_MAX, this));
    }

    // prix modif
    if (auto *prixMod = le(this, "l7")) {
        auto *dv = new QDoubleValidator(0.0, 1e12, 2, this);
        dv->setNotation(QDoubleValidator::StandardNotation);
        dv->setLocale(QLocale::system());
        prixMod->setValidator(dv);
    }

    // dispo modif
    if (auto *dispoMod = le(this, "l8")) {
        dispoMod->setValidator(new QIntValidator(0, 1, this));
    }

    /* ==== Modèle + tableView (LOCAL) ==== */
    setupModelAndView();

    /* ==== Carte dans graphicsView ==== */
    setupMapView();

    /* ==== Synchro sélection tableView -> formulaire + carte ==== */
    if (ui->tableView && ui->tableView->selectionModel()) {
        connect(ui->tableView->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this,
                &MainWindow::onTableCurrentRowChanged);
    }

    /* ==== Charger les réclamations au démarrage ==== */
    loadReclamationsIntoTable();

    /* ==== Contrôle de saisie sur l'état des réclamations (lineEdit_5) ==== */
    if (auto *leEtatReclam = le(this, "lineEdit_5")) {
        QRegularExpression rxEtat(
            "^\\s*(traite|traité|non[ _-]traite|non[ _-]traité)\\s*$",
            QRegularExpression::CaseInsensitiveOption
            );
        leEtatReclam->setValidator(
            new QRegularExpressionValidator(rxEtat, this)
            );
    }

    /* ==== Contrôle de saisie du délai (ajout + modif) ==== */
    {
        QRegularExpression rxDelai(
            "^\\s*\\d+\\s*(jour|jours|heure|heures|seconde|secondes)\\s*$",
            QRegularExpression::CaseInsensitiveOption
            );

        if (auto *leDelaiAdd = le(this, "lineEdit_6")) {
            leDelaiAdd->setValidator(
                new QRegularExpressionValidator(rxDelai, this)
                );
        }
        if (auto *leDelaiMod = le(this, "lineEdit_17")) {
            leDelaiMod->setValidator(
                new QRegularExpressionValidator(rxDelai, this)
                );
        }
    }
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
//         /////



void MainWindow::showReclamationDeadlinePopup(int code,
                                              const QString &nom,
                                              const QString &delai)
{
    // Fenêtre flottante stylée
    QDialog *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // Layouts
    auto *mainLayout   = new QVBoxLayout(dlg);
    auto *headerLayout = new QHBoxLayout();
    auto *bodyLayout   = new QVBoxLayout();

    // Header: icône + titre
    QLabel *iconLabel  = new QLabel("⚠", dlg);
    QLabel *titleLabel = new QLabel("Délai de réclamation dépassé", dlg);
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedWidth(40);

    titleLabel->setObjectName("titleLabel");

    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->setSpacing(8);

    // Corps: messages
    QLabel *msg1 = new QLabel(dlg);
    msg1->setText(
        QString("La réclamation <b>%1</b> du résident <b>%2</b> a dépassé le délai prévu.")
            .arg(code)
            .arg(nom)
        );

    QLabel *msg2 = new QLabel(dlg);
    msg2->setText(
        QString("Délai spécifié : <b>%1</b>.<br>"
                "Traite cette réclamation en priorité.")
            .arg(delai)
        );

    msg1->setWordWrap(true);
    msg2->setWordWrap(true);

    bodyLayout->addWidget(msg1);
    bodyLayout->addSpacing(4);
    bodyLayout->addWidget(msg2);

    // Boutons
    auto *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();

    QPushButton *btnOk    = new QPushButton("OK", dlg);
    QPushButton *btnFocus = new QPushButton("Voir les réclamations", dlg);

    buttonsLayout->addWidget(btnFocus);
    buttonsLayout->addWidget(btnOk);

    connect(btnOk, &QPushButton::clicked, dlg, &QDialog::accept);

    // Focus sur le tableau des réclamations
    connect(btnFocus, &QPushButton::clicked, this, [this, dlg, code]() {
        dlg->accept();

        // Si tu as un QTabWidget pour naviguer, tu peux le piloter ici, exemple :
        // ui->tabWidget->setCurrentIndex( indexDeLongletReclamation );

        if (ui->tableWidget) {
            for (int r = 0; r < ui->tableWidget->rowCount(); ++r) {
                auto *it = ui->tableWidget->item(r, 0);
                if (it && it->text().toInt() == code) {
                    ui->tableWidget->setCurrentCell(r, 0);
                    break;
                }
            }
        }
    });

    // Montage final
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(bodyLayout);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);

    dlg->setLayout(mainLayout);

    // Style rouge / sombre
    dlg->setStyleSheet(R"(
        QDialog {
            background-color: #2b0000;
            border-radius: 14px;
            border: 2px solid #ff5555;
        }
        QLabel#titleLabel {
            color: #ffdddd;
            font-size: 18px;
            font-weight: bold;
        }
        QLabel {
            color: #ffdede;
            font-size: 13px;
        }
        QPushButton {
            background-color: #660000;
            color: #ffdddd;
            border-radius: 8px;
            padding: 6px 14px;
            border: 1px solid #ff5555;
        }
        QPushButton:hover {
            background-color: #880000;
        }
        QPushButton:pressed {
            background-color: #550000;
        }
    )");

    dlg->adjustSize();

    // Centrer par rapport à la fenêtre principale
    const QRect parentGeom = this->geometry();
    const QPoint center    = parentGeom.center();
    const QRect dlgRect    = dlg->frameGeometry();
    dlg->move(center.x() - dlgRect.width() / 2,
              center.y() - dlgRect.height() / 2);

    dlg->show();
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

    if (!loc.insert(&err))  {
        QMessageBox::critical(this, "Échec insertion LOCAL", err);
        return;
    }

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

    // IMPORTANT: on ne logue plus l'ajout dans l'historique
    // writeLocalHistory(...); supprimé

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

    if (locationOK) {
        QMessageBox::information(this, "Succès", "Local et période de location ajoutés.");
    } else {
        QMessageBox::information(this, "Succès partiel",
                                 "Local ajouté, mais pas la période de location.");
    }

    refreshView();
}




//       //
void MainWindow::onSmsSent(bool success, const QString &message)
{
    // On ignore le texte brut de Twilio et on affiche un message propre
    if (success) {
        showSmsBanner("SMS envoyé avec succès ✅", true);
    } else {
        showSmsBanner("Échec de l'envoi du SMS ❌", false);
    }

    // Si tu veux loguer quand même dans la console :
    qDebug() << "[SMS]" << (success ? "OK:" : "ERREUR:") << message;
}



// window//




//nouv notif//







void MainWindow::onSmsError(const QString &error)
{
    QMessageBox::critical(this, "Erreur SMS", error);

    // Si tu veux loguer aussi dans l’historique, tu peux décommenter :
    /*
    if (m_histModel) {
        const int row = m_histModel->rowCount();
        m_histModel->insertRow(row);
        m_histModel->setData(m_histModel->index(row, 0),
                             QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss"));
        m_histModel->setData(m_histModel->index(row, 1),
                             "Erreur SMS : " + error);
    }
    */
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
    QLineEdit* idLE = le(this, "l5");
    if (!idLE) {
        QMessageBox::critical(this, "UI", "QLineEdit 'l5' introuvable.");
        return;
    }

    QString txt = idLE->text().trimmed();

    if (txt.isEmpty()) {
        refreshView();
        return;
    }

    QString sql = localSelectSql() + " WHERE ";
    QStringList conds;

    bool okId = false;
    int id = txt.toInt(&okId);
    if (okId && id > 0) {
        conds << QString("ID_LOCAL = %1").arg(id);
    }

    QString txtUpper = txt.toUpper();
    txtUpper.replace("'", "''");

    conds << QString("UPPER(TYPE_LOCAL) LIKE '%%1%'").arg(txtUpper);
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
    pdf.setPageOrientation(QPageLayout::Landscape); // paysage

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "PDF", "Impossible de créer le PDF.");
        return;
    }

    // Police principale du tableau
    QFont font("Arial", 12, QFont::Normal);
    painter.setFont(font);
    QFontMetrics fm(font);

    const int margin      = 40;
    const int usableWidth = pdf.width()  - 2 * margin;
    const int usableHeight= pdf.height() - 2 * margin;

    int y = margin;

    // ===== Titre =====
    QFont titleFont("Arial", 20, QFont::Bold);
    painter.setFont(titleFont);
    int titleHeight = QFontMetrics(titleFont).height() + 10;
    painter.drawText(QRect(margin, y, usableWidth, titleHeight),
                     Qt::AlignCenter, "Locaux non disponibles");
    y += titleHeight + 10;

    // ===== Date =====
    QFont dateFont("Arial", 10);
    painter.setFont(dateFont);
    int dateHeight = QFontMetrics(dateFont).height() + 6;
    const QString dateStr = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm");
    painter.drawText(QRect(margin, y, usableWidth, dateHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     "Généré le: " + dateStr);
    y += dateHeight + 15;

    // Retour police tableau
    painter.setFont(font);
    fm = QFontMetrics(font);

    // ===== Largeurs des colonnes =====
    const double wIdPct    = 0.06;
    const double wTypePct  = 0.12;
    const double wVillePct = 0.12;
    const double wAdrPct   = 0.28;
    const double wNomPct   = 0.22;
    const double wNumPct   = 0.09;
    const double wPrixPct  = 0.11;

    const int wId   = int(usableWidth * wIdPct);
    const int wType = int(usableWidth * wTypePct);
    const int wVille= int(usableWidth * wVillePct);
    const int wAdr  = int(usableWidth * wAdrPct);
    const int wNom  = int(usableWidth * wNomPct);
    const int wNum  = int(usableWidth * wNumPct);
    const int wPrix = usableWidth - (wId + wType + wVille + wAdr + wNom + wNum);

    const int cellPadding = 4;

    // ===== Lambda pour dessiner l'en-tête =====
    auto drawHeaderRow = [&](int &yPos) {
        QFont headerFont("Arial", 13, QFont::Bold);
        painter.setFont(headerFont);
        QFontMetrics fmHeader(headerFont);
        int headerHeight = fmHeader.height() + 10;

        int x = margin;

        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(QColor(173, 216, 230)); // bleu clair

        // ID
        painter.drawRect(x, yPos, wId, headerHeight);
        painter.drawText(QRect(x, yPos, wId, headerHeight),
                         Qt::AlignCenter, "ID");
        x += wId;

        // Type
        painter.drawRect(x, yPos, wType, headerHeight);
        painter.drawText(QRect(x, yPos, wType, headerHeight),
                         Qt::AlignCenter, "Type");
        x += wType;

        // Ville
        painter.drawRect(x, yPos, wVille, headerHeight);
        painter.drawText(QRect(x, yPos, wVille, headerHeight),
                         Qt::AlignCenter, "Ville");
        x += wVille;

        // Adresse
        painter.drawRect(x, yPos, wAdr, headerHeight);
        painter.drawText(QRect(x, yPos, wAdr, headerHeight),
                         Qt::AlignCenter, "Adresse");
        x += wAdr;

        // Propriétaire
        painter.drawRect(x, yPos, wNom, headerHeight);
        painter.drawText(QRect(x, yPos, wNom, headerHeight),
                         Qt::AlignCenter, "Propriétaire");
        x += wNom;

        // Numéro
        painter.drawRect(x, yPos, wNum, headerHeight);
        painter.drawText(QRect(x, yPos, wNum, headerHeight),
                         Qt::AlignCenter, "Numéro");
        x += wNum;

        // Prix
        painter.drawRect(x, yPos, wPrix, headerHeight);
        painter.drawText(QRect(x, yPos, wPrix, headerHeight),
                         Qt::AlignCenter, "Prix");

        yPos += headerHeight;     // prochaine ligne juste en dessous
        yPos += 5;                // petit espace

        painter.setFont(font);
        painter.setBrush(Qt::NoBrush);
    };

    drawHeaderRow(y);

    // ===== Lignes du tableau =====
    int rowHeight = fm.height() + 12;
    int pageBottom = margin + usableHeight;
    int count = 0;

    while (q.next()) {
        // Changement de page si besoin
        if (y + rowHeight > pageBottom) {
            pdf.newPage();
            y = margin + titleHeight + 10 + dateHeight + 15;
            drawHeaderRow(y);
        }

        const QString id      = q.value(0).toString();
        const QString type    = q.value(1).toString();
        const QString ville   = q.value(2).toString();
        const QString adresse = q.value(3).toString();
        const QString nom     = q.value(4).toString();
        const QString numero  = q.value(5).toString();
        const double  prix    = q.value(6).toDouble();
        const QString prixTxt = QLocale().toString(prix, 'f', 2) + " TND";

        int x = margin;

        painter.setPen(QPen(Qt::black, 1));
        painter.setBrush(Qt::NoBrush);

        // ID
        painter.drawRect(x, y, wId, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wId - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         id);
        x += wId;

        // Type
        painter.drawRect(x, y, wType, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wType - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         type);
        x += wType;

        // Ville
        painter.drawRect(x, y, wVille, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wVille - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         ville);
        x += wVille;

        // Adresse
        painter.drawRect(x, y, wAdr, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wAdr - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         adresse);
        x += wAdr;

        // Propriétaire
        painter.drawRect(x, y, wNom, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wNom - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         nom);
        x += wNom;

        // Numéro
        painter.drawRect(x, y, wNum, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wNum - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignLeft,
                         numero);
        x += wNum;

        // Prix
        painter.drawRect(x, y, wPrix, rowHeight);
        painter.drawText(QRect(x + cellPadding, y,
                               wPrix - 2*cellPadding, rowHeight),
                         Qt::AlignVCenter | Qt::AlignRight,
                         prixTxt);

        y += rowHeight;   // ligne suivante
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
            QString("PDF généré avec succès!\n%1\n%2 local(aux) listé(s).")
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

    m_histModel->removeRows(0, m_histModel->rowCount());

    int row = 0;
    for (const QString &ligne : lignes) {
        const QString dateString = ligne.section('|', 0, 0).trimmed();
        QDateTime date = QDateTime::fromString(dateString, "dd/MM/yyyy hh:mm:ss");
        if (!date.isValid())
            continue;

        if (date < ilyaUnMois)
            continue;

        QString action = ligne.section('|', 1).trimmed();
        if (action.isEmpty())
            action = "(action vide)";

        // Ici on ignore les actions d'ajout dans l'affichage de l'historique
        if (action.startsWith("AJOUT LOCAL", Qt::CaseInsensitive))
            continue;

        m_histModel->insertRow(row);
        m_histModel->setData(m_histModel->index(row, 0),
                             date.toString("dd/MM/yyyy hh:mm:ss"));
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
    auto *leEtat  = le(this, "lineEdit_5");  // traité / non traité (texte)
    auto *leDelai = le(this, "lineEdit_6");

    if (!leCode || !leDate || !leNom || !leDesc || !leEtat || !leDelai) {
        QMessageBox::critical(this, "UI", "Certains QLineEdit sont introuvables.");
        return;
    }

    // --- CODE ---
    bool okInt = false;
    const int code = leCode->text().trimmed().toInt(&okInt);
    if (!okInt || code <= 0) {
        QMessageBox::warning(this, "Validation", "CODE doit être un entier > 0.");
        return;
    }

    // --- DATE ---
    QDate d;
    if (!parseDateReclamation(leDate->text(), d)) {
        QMessageBox::warning(this, "Validation",
                             "Date invalide. Ex: 31/12/2025, 2025-12-31, 31-12-2025.");
        return;
    }

    // --- CHAMPS TEXTE ---
    const QString nom   = leNom->text().trimmed();
    const QString desc  = leDesc->text().trimmed();
    const QString delai = leDelai->text().trimmed();

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Validation", "NOM_RESIDENT est obligatoire.");
        return;
    }
    if (desc.isEmpty()) {
        QMessageBox::warning(this, "Validation", "DESCRIPTION est obligatoire.");
        return;
    }

    // --- ETAT : "traité" / "non traité" ---
    QString etatUser = leEtat->text().trimmed();

    etatUser = etatUser.normalized(QString::NormalizationForm_D);
    etatUser.remove(QRegularExpression(u8"[\\p{Mn}]")); // enlever accents
    etatUser = etatUser.toLower();
    etatUser.remove(' ');
    etatUser.remove('-');
    etatUser.remove('_');

    QString etatBd;
    if (etatUser == "traite") {
        etatBd = "CLOSE";
    } else if (etatUser == "nontraite") {
        etatBd = "OUVERTE";
    } else {
        QMessageBox::warning(
            this,
            "État invalide",
            "Dans l'état, tape seulement \"traité\" ou \"non traité\"."
            );
        return;
    }

    // --- DÉLAI ---
    if (!delai.isEmpty()) {
        QRegularExpression rxDelai(
            "^\\s*(\\d+)\\s*(jour|jours|heure|heures|seconde|secondes)\\s*$",
            QRegularExpression::CaseInsensitiveOption
            );
        QRegularExpressionMatch m = rxDelai.match(delai);
        if (!m.hasMatch()) {
            QMessageBox::warning(
                this,
                "Délai invalide",
                "Format du délai attendu : nombre + espace + unité.\n"
                "Exemples : \"3 jours\", \"2 heures\", \"10 secondes\"."
                );
            return;
        }
    }

    // --- Objet métier ---
    Reclamation r(code, d, nom, desc, etatBd, delai);

    QString err;
    if (!r.isValid(&err)) {
        QMessageBox::warning(this, "Champs invalides", err);
        return;
    }

    if (!r.insert(&err)) {
        if (err.contains("ORA-02290"))
            err = "ETAT invalide en base. Valeurs autorisées: OUVERTE, EN_COURS, CLOSE.";
        if (err.contains("ORA-00001"))
            err = "CODE existe déjà (clé unique).";
        if (err.contains("ORA-04098"))
            err = "Trigger invalide sur RECLAMATION. Corrige BI_RECLAMATION côté Oracle.";
        QMessageBox::critical(this, "Insertion", err);
        return;
    }

    QMessageBox::information(this, "Succès", "Réclamation ajoutée.");

    // 1) SMS AUTOMATIQUE
    if (m_smsManager) {
        const QString numero = "93341398";         // à remplacer par le vrai numéro
        const QString codeStr = QString::number(code);

        m_smsManager->envoyerSMSReclamation(numero,
                                            nom,
                                            codeStr,
                                            desc,
                                            delai);
    }

    // 2) NOTIFICATION WINDOWS
    showReclamationNotification(nom,
                                QString::number(code),
                                delai);

    // 3) TIMER DÉLAI DÉPASSÉ
    if (!delai.isEmpty()) {
        bool okDelay = false;
        qint64 delayMs = delaiToMs(delai, &okDelay);

        if (okDelay && delayMs > 0) {
            const int codeReclam        = code;
            const QString nomReclam     = nom;
            const QString delaiReclam   = delai;
            const QString etatReclamBd  = etatBd;

            QTimer::singleShot(delayMs, this,
                               [this, codeReclam, nomReclam, delaiReclam, etatReclamBd]() {

                                   QSqlQuery q(Connection::db());
                                   q.prepare("SELECT ETAT FROM FIRAS.RECLAMATION WHERE CODE = :c");
                                   q.bindValue(":c", codeReclam);
                                   if (q.exec() && q.next()) {
                                       const QString etatActuel = q.value(0).toString();
                                       if (etatActuel != "OUVERTE" && etatActuel != etatReclamBd) {
                                           return;
                                       }
                                   }

                                   if (m_smsManager) {
                                       const QString msg =
                                           QString("Réclamation %1 (%2) : délai %3 dépassé.")
                                               .arg(codeReclam)
                                               .arg(nomReclam)
                                               .arg(delaiReclam);

                                       const QString numero = "93341398";
                                       m_smsManager->envoyerSMS(numero, msg);
                                   }

                                   this->showReclamationDeadlinePopup(codeReclam,
                                                                      nomReclam,
                                                                      delaiReclam);
                               });
        }
    }

    // Nettoyage UI
    leCode->clear();
    leNom->clear();
    leDesc->clear();
    leEtat->clear();
    leDelai->clear();
    leDate->clear();

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

    // Vérification du format du délai aussi en modification
    if (!delai.isEmpty()) {
        QRegularExpression rxDelai(
            "^\\s*(\\d+)\\s*(jour|jours|heure|heures|seconde|secondes)\\s*$",
            QRegularExpression::CaseInsensitiveOption
            );
        QRegularExpressionMatch m = rxDelai.match(delai);
        if (!m.hasMatch()) {
            QMessageBox::warning(
                this,
                "Délai invalide",
                "Format attendu: nombre + espace + unité (jour(s), heure(s), seconde(s))."
                );
            return;
        }
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

    const int total = nbDispo + nbNonDispo;
    if (total == 0) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucun local trouvé dans la base.");
        return;
    }

    QPieSeries *series = new QPieSeries();
    QPieSlice *sliceDispo    = series->append("Disponibles",     nbDispo);
    QPieSlice *sliceNonDispo = series->append("Non disponibles", nbNonDispo);

    QColor colorDispo("#5cb5f2");
    QColor colorNonDispo("#435AD9");

    sliceDispo->setBrush(colorDispo);
    sliceNonDispo->setBrush(colorNonDispo);

    for (QPieSlice *s : series->slices()) {
        const double pct = (s->value() * 100.0) / total;
        s->setLabel(QString("%1%").arg(pct, 0, 'f', 1));
        s->setLabelVisible(true);
        s->setLabelPosition(QPieSlice::LabelOutside);
    }
    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des locaux");
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // widget légende en haut
    auto createLegendItem = [](const QString &text, const QColor &color) {
        QWidget *w = new QWidget;
        QHBoxLayout *l = new QHBoxLayout(w);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(5);

        QLabel *colorLabel = new QLabel;
        colorLabel->setFixedSize(16, 16);
        colorLabel->setStyleSheet(
            QString("background-color: %1; border: 1px solid black;")
                .arg(color.name())
            );

        QLabel *textLabel = new QLabel(text);

        l->addWidget(colorLabel);
        l->addWidget(textLabel);

        return w;
    };

    QWidget *legendWidget = new QWidget(this);
    QHBoxLayout *legendLayout = new QHBoxLayout(legendWidget);
    legendLayout->setContentsMargins(0, 0, 0, 0);
    legendLayout->setSpacing(10);

    legendLayout->addWidget(createLegendItem("Disponible",     colorDispo));
    legendLayout->addWidget(createLegendItem("Non disponible", colorNonDispo));
    legendLayout->addStretch(); // pousse tout à gauche

    // container pour légende + graphique
    QWidget *container = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->setSpacing(5);
    containerLayout->addWidget(legendWidget);
    containerLayout->addWidget(chartView);

    if (ui->verticalLayout) {
        // si tu veux vider avant d’afficher ces stats, tu gardes ça
        QLayoutItem *item;
        while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
            if (item->widget())
                item->widget()->deleteLater();
            delete item;
        }

        // tout ce bloc (carrés + camembert) est dans la zone de verticalLayout
        ui->verticalLayout->addWidget(container);
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
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

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

    QSplineSeries *series = new QSplineSeries();
    series->setName("Nombre de locaux par type");

    for (int i = 0; i < counts.size(); ++i) {
        series->append(i, counts[i]);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des locaux par type (courbe)");

    QCategoryAxis *axisX = new QCategoryAxis();
    axisX->setTitleText("Type de local");

    for (int i = 0; i < types.size(); ++i) {
        axisX->append(types[i], i);
    }
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de locaux");
    axisY->setLabelFormat("%d");
    axisY->setRange(0, maxCount + 1);

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

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

// les metiers de azyz //

/* ---- Recherche par ETAT (traité / non traité / états BD) via lineEdit ---- */
void MainWindow::on_pushButton_8_clicked()
{
    QLineEdit *leEtat = this->findChild<QLineEdit*>("lineEdit");
    if (!leEtat) {
        QMessageBox::critical(this, "UI",
                              "QLineEdit 'lineEdit' introuvable (recherche ETAT).");
        return;
    }

    const QString texteBrut = leEtat->text().trimmed();

    if (texteBrut.isEmpty()) {
        loadReclamationsIntoTable();
        return;
    }

    const QString etatCanon = canonEtat(texteBrut);
    if (etatCanon != "OUVERTE" && etatCanon != "EN_COURS" && etatCanon != "CLOSE") {
        QMessageBox::warning(
            this,
            "État invalide",
            "État inconnu. Utilise \"traité\", \"non traité\", OUVERTE, EN_COURS ou CLOSE."
            );
        return;
    }

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

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
            "WHERE ETAT = :etat "
            "ORDER BY DATE_RECLAMATION ASC, CODE ASC"
            ).arg(tableName);

    QSqlQuery q(db);
    if (!q.prepare(sql)) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Préparation SELECT échouée: " + q.lastError().text());
        return;
    }

    q.bindValue(":etat", etatCanon);

    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "SELECT échoué: " + q.lastError().text());
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
        QMessageBox::information(this, "Recherche",
                                 "Aucune réclamation trouvée pour cet état.");
    }
}

/* ---- Tri des réclamations par ETAT ---- */
void MainWindow::on_pushButton_9_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

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
                            "ORDER BY ETAT ASC, DATE_RECLAMATION ASC, CODE ASC"
                            ).arg(tableName);

    QSqlQuery q(db);
    if (!q.exec(sql)) {
        const QString e = q.lastError().text();
        QMessageBox::critical(this, "Erreur", "Lecture réclamations (tri état): " + e);
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
        QMessageBox::information(this, "Tri par état",
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

/* ---- Statistiques par ETAT en camembert (STATISTIQUEBUTTON_11) ---- */
void MainWindow::on_statistiqueButton_11_clicked()
{
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

    QSqlQuery q(db);
    if (!q.exec(QStringLiteral(
                    "SELECT ETAT, COUNT(*) "
                    "FROM %1 "
                    "GROUP BY ETAT"
                    ).arg(tableName))) {
        QMessageBox::critical(this, "Statistiques",
                              "Erreur SQL: " + q.lastError().text());
        return;
    }

    QMap<QString, int> mapEtatCount;
    int total = 0;

    while (q.next()) {
        const QString etat = q.value(0).toString();
        const int nb       = q.value(1).toInt();
        mapEtatCount[etat] = nb;
        total += nb;
    }

    if (total == 0) {
        QMessageBox::information(this, "Statistiques",
                                 "Aucune réclamation trouvée dans la base.");
        return;
    }

    QPieSeries *series = new QPieSeries();

    for (auto it = mapEtatCount.constBegin(); it != mapEtatCount.constEnd(); ++it) {
        const QString etat = it.key();
        const int nb       = it.value();
        QPieSlice *slice   = series->append(etat, nb);

        const double pct = (nb * 100.0) / total;
        slice->setLabel(QString("%1 : %2%")
                            .arg(etat)
                            .arg(pct, 0, 'f', 1));
        slice->setLabelVisible(true);
        slice->setLabelPosition(QPieSlice::LabelOutside);
    }

    series->setLabelsVisible(true);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des réclamations par état");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

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

/* ---- PDF réclamations par état (pushButton_10) ---- */
void MainWindow::on_pushButton_10_clicked()
{
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

    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION");

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
        r[0] = q.value(0).toString();
        r[1] = q.value(1).toString();
        r[2] = q.value(2).toString();
        r[3] = q.value(3).toString();
        r[4] = q.value(4).toString();
        r[5] = q.value(5).toString();
        rows.push_back(r);
    }

    if (rows.isEmpty()) {
        QMessageBox::information(this, "PDF",
                                 "Aucune réclamation trouvée pour l'état " + etat + ".");
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

    QString safeEtat = etat;
    safeEtat.replace(' ', '_');
    safeEtat.replace('/', '_');

    const QString filePath =
        baseDir.filePath(QString("reclamations_%1.pdf").arg(safeEtat));

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

    const int wCode = int(usableWidth * 0.08);
    const int wDate = int(usableWidth * 0.12);
    const int wNom  = int(usableWidth * 0.18);
    const int wDesc = int(usableWidth * 0.40);
    const int wEtat = int(usableWidth * 0.10);
    const int wDelai= usableWidth - (wCode + wDate + wNom + wDesc + wEtat);

    auto drawHeaderRow = [&](int& yPos) {
        QFont headerFont("Helvetica", 11, QFont::Bold);
        painter.setFont(headerFont);

        int x = margin;
        const int headerHeight = 32;

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

        yPos += headerHeight + 10;
        painter.setFont(font);
    };

    drawHeaderRow(y);

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
        rowHeight += 12;

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

void MainWindow::on_pushButton_testSms_clicked()
{
    if (!m_smsManager) {
        showSmsBanner("SMSManager non initialisé.", false);
        return;
    }

    // Numéro qui doit recevoir le SMS
    const QString numero = "93341398";

    // Message de test
    const QString msg =
        QStringLiteral("Test SMS depuis l'application de gestion des réclamations.");

    const bool started = m_smsManager->envoyerSMS(numero, msg);

    if (!started) {
        // Erreur locale (credentials Twilio, réseau, etc.)
        showSmsBanner("Échec immédiat de l'envoi du SMS de test.", false);
    } else {
        // La requête HTTP part, la réponse finale arrivera dans onSmsSent / onSmsError
        showSmsBanner("Envoi du SMS de test en cours...", true);
    }
}
//....//




void MainWindow::showSmsBanner(const QString &text, bool success)
{
    // Petit widget flottant dans la fenêtre principale
    QWidget *banner = new QWidget(this);
    banner->setAttribute(Qt::WA_DeleteOnClose);
    banner->setObjectName("smsBanner");

    auto *layout = new QHBoxLayout(banner);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(8);

    QLabel *icon = new QLabel(banner);
    icon->setText(success ? "✅" : "⚠️");
    icon->setAlignment(Qt::AlignCenter);

    QLabel *label = new QLabel(text, banner);
    label->setWordWrap(false);
    label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    layout->addWidget(icon);
    layout->addWidget(label);
    banner->setLayout(layout);

    // Ombre
    auto *shadow = new QGraphicsDropShadowEffect(banner);
    shadow->setBlurRadius(20);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 160));
    banner->setGraphicsEffect(shadow);

    // Style vert si succès, rouge si échec
    const QString bgColor     = success ? "#1e7d32" : "#b71c1c";
    const QString borderColor = success ? "#66bb6a" : "#ef5350";

    banner->setStyleSheet(QString(R"(
        QWidget#smsBanner {
            background-color: %1;
            border-radius: 10px;
            border: 1px solid %2;
        }
        QLabel {
            color: #f5f5f5;
            font-size: 12px;
        }
    )").arg(bgColor, borderColor));

    banner->adjustSize();

    // Position haut droite
    const int margin = 20;
    QRect r = this->rect();

    int x = r.right() - banner->width() - margin;
    int y = r.top() + margin;

    banner->move(x, y);
    banner->show();

    // Disparition après 3s
    QTimer::singleShot(3000, banner, &QWidget::close);
}

void MainWindow::showReclamationNotification(const QString &nomResident,
                                             const QString &codeReclamation,
                                             const QString &delai)
{
    if (!m_trayIcon || !QSystemTrayIcon::isSystemTrayAvailable())
        return;

    QString title = "Réclamation enregistrée";

    QString body = QString("Bonjour %1,\n"
                           "Votre réclamation avec le code %2 a été enregistrée avec succès.")
                       .arg(nomResident, codeReclamation);

    if (!delai.trimmed().isEmpty()) {
        body += QString("\nDélai estimé de traitement : %1.").arg(delai);
    }

    body += "\nMerci de votre confiance.";

    m_trayIcon->showMessage(
        title,
        body,
        QSystemTrayIcon::Information,
        5000
        );
}
