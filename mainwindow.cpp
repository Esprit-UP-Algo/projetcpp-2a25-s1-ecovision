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
    if (ok) { if (out) *out = v; return true; }

    QString n = txt;
    n.remove(' ');
    n.remove(QChar(0x00A0));
    n.remove(QChar(0x202F));
    n.replace(',', '.');
    v = n.toDouble(&ok);
    if (ok) { if (out) *out = v; return true; }

    static const QRegularExpression re(R"(([+-]?\d{1,3}(?:[ \u00A0\u202F]?\d{3})*(?:[.,]\d+)?|[+-]?\d+(?:[.,]\d+)?))");
    auto m = re.match(txt);
    if (!m.hasMatch()) return false;

    QString num = m.captured(1);
    num.remove(' '); num.remove(QChar(0x00A0)); num.remove(QChar(0x202F));
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

/* ---- MainWindow ---- */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Optionnel: force le schéma si ta connexion n’a pas FIRAS par défaut
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

    setupModelAndView();

    // ID de modif à la sélection dans la table des Local
    if (ui->tableView->selectionModel()) {
        connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, [this](const QModelIndex& current, const QModelIndex&) {
                    if (!current.isValid()) return;
                    const QVariant idv = ui->tableView->model()->index(current.row(), 0).data();
                    if (auto *idLE = this->findChild<QLineEdit*>("lineedit_id_modif"))
                        idLE->setText(idv.toString());
                });
    }

    // Charge les réclamations au démarrage
    loadReclamationsIntoTable();
}

MainWindow::~MainWindow() { delete ui; }

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

/* ---- Ajouter Local ---- */
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

    Local loc(
        le(this, "l1")->text().trimmed(),
        le(this, "l2")->text().trimmed(),
        le(this, "l3")->text().trimmed(),
        dispo,
        le(this, "l4_2")->text().trimmed(),
        le(this, "l4_5")->text().trimmed(),
        le(this, "l4_3")->text().trimmed(),
        prix
        );

    QString err;
    if (!loc.isValid(&err)) {
        QMessageBox::warning(this, "Champs invalides", err);
        return;
    }
    if (!loc.insert(&err))  {
        QMessageBox::critical(this, "Échec insertion", err);
        return;
    }

    le(this, "l1")->clear();  le(this, "l2")->clear();  le(this, "l3")->clear();  le(this, "l4")->clear();
    le(this, "l4_2")->clear(); le(this, "l4_5")->clear(); le(this, "l4_3")->clear(); le(this, "l4_4")->clear();

    QMessageBox::information(this, "Succès", "Local ajouté.");
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

    prixLE->clear();
    dispoLE->clear();

    QMessageBox::information(this, "Succès", "Local modifié.");
    refreshView();
}

/* ---- Supprimer Local ---- */
void MainWindow::on_supprimerButton_clicked()
{
    int id = 0; bool okId = false;

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
                if (ok2 && id2 > 0) { id = id2; okId = true; }
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

    if (auto *idLE = le(this, "lineedit_id_modif")) idLE->clear();
    if (ui->tableView->selectionModel()) ui->tableView->selectionModel()->clearSelection();

    QMessageBox::information(this, "Succès", "Suppression effectuée.");
    refreshView();
}

/* ---- Ajouter Réclamation (pushButton_13) ---- */
void MainWindow::on_pushButton_13_clicked()
{
    auto *leCode  = le(this, "lineEdit_12"); // CODE
    auto *leDate  = le(this, "lineEdit_2");  // DATE_RECLAMATION
    auto *leNom   = le(this, "lineEdit_3");  // NOM_RESIDENT
    auto *leDesc  = le(this, "lineEdit_4");  // DESCRIPTION
    auto *leEtat  = le(this, "lineEdit_5");  // ETAT
    auto *leDelai = le(this, "lineEdit_6");  // DELAI

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
    leCode->clear(); leNom->clear(); leDesc->clear(); leEtat->clear(); leDelai->clear();

    loadReclamationsIntoTable();
}

/* ---- Affichage table RECLAMATION dans QTableWidget ---- */
void MainWindow::loadReclamationsIntoTable()
{
    const QString tableName = QStringLiteral("FIRAS.RECLAMATION"); // ou "RECLAMATION" si CURRENT_SCHEMA=FIRAS
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

    // Si ton widget a un autre nom, remplace ici "tableWidget"
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
    // Préparer la combo des états autorisés
    if (ui->comboBox_2) {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems({"OUVERTE","EN_COURS","CLOSE"});
    }

    // Brancher la sélection du QTableWidget pour remplir le formulaire d'édition
    if (ui->tableWidget) {
        connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
                this, &MainWindow::syncReclamSelectionToForm);
    }


    tw->resizeColumnsToContents();
}
void MainWindow::on_pushButton_12_clicked()
{
    // Récupérer le QTableWidget (adapte si ton widget a un autre nom)
    QTableWidget* tw = ui->tableWidget;
    if (!tw) {
        QMessageBox::critical(this, "UI", "QTableWidget 'tableWidget' introuvable.");
        return;
    }

    // Récupérer les lignes sélectionnées (indices de lignes)
    const auto sel = tw->selectionModel()
                         ? tw->selectionModel()->selectedRows()
                         : QModelIndexList{};
    if (sel.isEmpty()) {
        QMessageBox::information(this, "Suppression", "Sélectionne une ligne d'abord.");
        return;
    }

    // On autorise la multi-sélection au cas où tu l'actives plus tard
    // On collecte les lignes et les CODE à supprimer
    QVector<int> rows;
    rows.reserve(sel.size());
    QVector<int> codes;
    codes.reserve(sel.size());

    for (const QModelIndex& mi : sel) {
        const int row = mi.row();
        rows.push_back(row);

        // Colonne 0 = Code (voir loadReclamationsIntoTable)
        QTableWidgetItem* itCode = tw->item(row, 0);
        bool ok = false;
        const int code = itCode ? itCode->text().toInt(&ok) : 0;
        if (!ok || code <= 0) {
            QMessageBox::warning(this, "Suppression", "CODE invalide dans la ligne sélectionnée.");
            return;
        }
        codes.push_back(code);
    }

    // Supprimer en base
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "DB", "Connexion base indisponible.");
        return;
    }

    // Si tu n'as pas mis CURRENT_SCHEMA=FIRAS à la connexion, préfixe la table ici:
    const QString tableName = QStringLiteral("FIRAS.RECLAMATION"); // ou "RECLAMATION" si CURRENT_SCHEMA=FIRAS

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
            QMessageBox::critical(this, "Suppression", "Échec suppression (CODE="
                                                           + QString::number(code) + "): " + err);
            return;
        }
    }

    if (!db.commit()) {
        QMessageBox::critical(this, "DB", "Commit échoué.");
        return;
    }

    // Retirer les lignes du widget (du bas vers le haut pour préserver les indices)
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    for (int r : rows)
        tw->removeRow(r);

    // Optionnel: recharger pour rester 100% synchro avec la base
    // (utile si d'autres colonnes/tri changent côté DB)
    // loadReclamationsIntoTable();
}
void MainWindow::syncReclamSelectionToForm()
{
    QTableWidget* tw = ui->tableWidget;
    if (!tw) return;

    const auto sel = tw->selectionModel()
                         ? tw->selectionModel()->selectedRows()
                         : QModelIndexList{};
    if (sel.isEmpty()) return;

    const int row = sel.first().row();

    // Récupérer les valeurs de la ligne (0..5) comme construites dans loadReclamationsIntoTable()
    const QString code  = tw->item(row, 0) ? tw->item(row, 0)->text().trimmed() : QString();
    const QString date  = tw->item(row, 1) ? tw->item(row, 1)->text().trimmed() : QString(); // YYYY-MM-DD
    const QString nom   = tw->item(row, 2) ? tw->item(row, 2)->text().trimmed() : QString();
    const QString desc  = tw->item(row, 3) ? tw->item(row, 3)->toolTip().isEmpty()
                                                ? tw->item(row, 3)->text().trimmed()
                                                : tw->item(row, 3)->toolTip() // on récupère le plein texte si dispo
                                          : QString();
    const QString etat  = tw->item(row, 4) ? tw->item(row, 4)->text().trimmed() : QString();
    const QString delai = tw->item(row, 5) ? tw->item(row, 5)->text().trimmed() : QString();

    if (auto *le = this->findChild<QLineEdit*>("lineEdit_13")) le->setText(code);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_14")) le->setText(date);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_15")) le->setText(nom);
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_16")) le->setText(desc);
    if (ui->comboBox_2) {
        int idx = ui->comboBox_2->findText(etat, Qt::MatchFixedString);
        if (idx < 0) idx = 0; // par défaut OUVERTE
        ui->comboBox_2->setCurrentIndex(idx);
    }
    if (auto *le = this->findChild<QLineEdit*>("lineEdit_17")) le->setText(delai);
}


void MainWindow::on_pushButton_17_clicked()
{
    // Lire les champs
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
    const QString etat  = cbEtat->currentText().trimmed();         // valeurs autorisées via la combo
    const QString delai = leDelai->text().trimmed();

    if (nom.isEmpty())  { QMessageBox::warning(this, "Validation", "NOM_RESIDENT est obligatoire."); return; }
    if (desc.isEmpty()) { QMessageBox::warning(this, "Validation", "DESCRIPTION est obligatoire."); return; }

    // Update DB
    QSqlDatabase db = Connection::db();
    if (!db.isOpen()) { QMessageBox::critical(this, "DB", "Connexion base indisponible."); return; }

    const QString tableName = QStringLiteral("FIRAS.RECLAMATION"); // "RECLAMATION" si CURRENT_SCHEMA=FIRAS
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

    q.bindValue(":d", d.startOfDay());   // Qt 6
    q.bindValue(":n", nom);
    q.bindValue(":x", desc);
    q.bindValue(":e", etat);             // limité par la combo aux 3 valeurs
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

    // Rafraîchir l’affichage et reselectionner le code
    loadReclamationsIntoTable();

    // Reselectionner la ligne mise à jour dans le tableau
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

