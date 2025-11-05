// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "local.h"

#include <QMessageBox>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QLocale>
#include <QRegularExpression>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <climits>
#include <optional>

/* ---- Helpers ---- */
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

/* ---- MainWindow ---- */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Validateurs
    if (auto *idLE = le(this, "lineedit_id_modif"))
        idLE->setValidator(new QIntValidator(1, INT_MAX, this));
    if (auto *i8 = le(this, "l8"))
        i8->setValidator(new QIntValidator(0, 1, this));
    if (auto *i7 = le(this, "l7")) {
        auto *dv = new QDoubleValidator(0.0, 1e9, 2, this);
        dv->setNotation(QDoubleValidator::StandardNotation);
        dv->setLocale(QLocale::system());
        i7->setValidator(dv);
        if (i7->placeholderText().isEmpty())
            i7->setPlaceholderText("Entrez le prix");
    }

    // Secours si l’autoconnect ne trouve pas le slot
    if (auto *btn = this->findChild<QPushButton*>("supprimerButton"))
        connect(btn, &QPushButton::clicked, this, &MainWindow::on_supprimerButton_clicked);

    // Modèle + vue
    setupModelAndView();

    // Remplir automatiquement l’ID quand on change de ligne dans le tableau
    if (ui->tableView->selectionModel()) {
        connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, [this](const QModelIndex& current, const QModelIndex&) {
                    if (!current.isValid() || !m_model) return;
                    int idCol = m_model->fieldIndex("ID_LOCAL");
                    if (idCol < 0) idCol = 0; // fallback
                    const QVariant idv = m_model->data(m_model->index(current.row(), idCol));
                    if (auto *idLE = this->findChild<QLineEdit*>("lineedit_id_modif"))
                        idLE->setText(idv.toString());
                });
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupModelAndView()
{
    m_model = new QSqlTableModel(this, QSqlDatabase::database());
    m_model->setTable("LOCAL");
    m_model->setEditStrategy(QSqlTableModel::OnFieldChange);
    m_model->select();

    // Ordre attendu: ID_LOCAL, TYPE_LOCAL, VILLE, ADRESSE, DISPONIBILITE, NOM_PROPRIETAIRE, CIN_PROPRIETAIRE, NUMERO, PRIX
    m_model->setHeaderData(0, Qt::Horizontal, "ID");
    m_model->setHeaderData(1, Qt::Horizontal, "Type");
    m_model->setHeaderData(2, Qt::Horizontal, "Ville");
    m_model->setHeaderData(3, Qt::Horizontal, "Adresse");
    m_model->setHeaderData(4, Qt::Horizontal, "Dispo");
    m_model->setHeaderData(5, Qt::Horizontal, "Nom prop.");
    m_model->setHeaderData(6, Qt::Horizontal, "CIN prop.");
    m_model->setHeaderData(7, Qt::Horizontal, "Numéro");
    m_model->setHeaderData(8, Qt::Horizontal, "Prix");

    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::refreshView()
{
    if (m_model) m_model->select();
}

/* ---- Ajouter ---- */
void MainWindow::on_ajouterButton_clicked()
{
    bool okInt = false, okDouble = false;
    const int dispo = ui->l4->text().trimmed().toInt(&okInt);
    double p = 0.0;
    okDouble = parsePrixRobuste(ui->l4_4->text(), &p);

    if (!okInt || (dispo != 0 && dispo != 1)) {
        QMessageBox::warning(this, "Champs invalides", "Disponibilité doit être 0 ou 1.");
        return;
    }
    if (!okDouble || p < 0) {
        QMessageBox::warning(this, "Champs invalides", "Prix doit être un nombre positif.");
        return;
    }

    Local loc(
        ui->l1->text().trimmed(),
        ui->l2->text().trimmed(),
        ui->l3->text().trimmed(),
        dispo,
        ui->l4_2->text().trimmed(),
        ui->l4_5->text().trimmed(),
        ui->l4_3->text().trimmed(),
        p
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

    ui->l1->clear();  ui->l2->clear();  ui->l3->clear();  ui->l4->clear();
    ui->l4_2->clear(); ui->l4_5->clear(); ui->l4_3->clear(); ui->l4_4->clear();

    QMessageBox::information(this, "Succès", "Local ajouté.");
    refreshView();
}

/* ---- Modifier ---- */
void MainWindow::on_modifierButton_clicked()
{
    auto *idLE = le(this, "lineedit_id_modif");
    auto *i7   = le(this, "l7");   // prix
    auto *i8   = le(this, "l8");   // dispo

    if (!idLE || !i7 || !i8) {
        QMessageBox::critical(this, "Erreur UI", "lineedit_id_modif, l7 et l8 doivent exister dans l'UI.");
        return;
    }

    bool okId = false;
    const int id = idLE->text().trimmed().toInt(&okId);
    if (!okId || id <= 0) {
        QMessageBox::warning(this, "ID invalide", "ID_LOCAL doit être > 0.");
        return;
    }

    const QString prixTxt  = i7->text().trimmed();
    const QString dispoTxt = i8->text().trimmed();
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

    i7->clear();
    i8->clear();

    QMessageBox::information(this, "Succès", "Local modifié.");
    refreshView();
}

/* ---- Supprimer (robuste) ---- */
void MainWindow::on_supprimerButton_clicked()
{
    int id = 0;
    bool okId = false;

    // 1) Tente depuis le champ
    if (auto *idLE = this->findChild<QLineEdit*>("lineedit_id_modif"))
        id = idLE->text().trimmed().toInt(&okId);

    // 2) Sinon, depuis la ligne sélectionnée
    if ((!okId || id <= 0) && ui->tableView && ui->tableView->model()) {
        QModelIndex current = ui->tableView->currentIndex();
        if (!current.isValid() && ui->tableView->selectionModel())
            current = ui->tableView->selectionModel()->currentIndex();

        if (current.isValid()) {
            int idCol = m_model ? m_model->fieldIndex("ID_LOCAL") : -1;
            if (idCol < 0) idCol = 0; // fallback
            const QVariant idv = ui->tableView->model()->index(current.row(), idCol).data();
            bool ok2 = false;
            const int id2 = idv.toString().trimmed().toInt(&ok2);
            if (ok2 && id2 > 0) { id = id2; okId = true; }
        }
    }

    if (!okId || id <= 0) {
        QMessageBox::warning(this, "ID invalide", "ID_LOCAL doit être > 0.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              QString("Supprimer l'élément ID %1 ?").arg(id),
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    QString err;
    if (!Local::deleteById(id, &err)) {
        QMessageBox::critical(this, "Échec suppression", err);
        return;
    }

    if (auto *idLE = this->findChild<QLineEdit*>("lineedit_id_modif"))
        idLE->clear();
    if (ui->tableView->selectionModel())
        ui->tableView->selectionModel()->clearSelection();

    QMessageBox::information(this, "Succès", "Suppression effectuée.");
    refreshView();
}
