#include "verifycodedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

VerifyCodeDialog::VerifyCodeDialog(const QString &prefillCin, QWidget *parent)
    : QDialog(parent), m_cinEdit(new QLineEdit(this)), m_codeEdit(new QLineEdit(this))
{
    setWindowTitle(tr("Vérification par code"));
    QVBoxLayout *main = new QVBoxLayout(this);

    QHBoxLayout *cinRow = new QHBoxLayout;
    cinRow->addWidget(new QLabel(tr("CIN:"), this));
    m_cinEdit->setPlaceholderText(tr("Entrez votre CIN (8 chiffres)"));
    if (!prefillCin.isEmpty()) m_cinEdit->setText(prefillCin);
    cinRow->addWidget(m_cinEdit);
    main->addLayout(cinRow);

    QHBoxLayout *codeRow = new QHBoxLayout;
    codeRow->addWidget(new QLabel(tr("Code:"), this));
    m_codeEdit->setPlaceholderText(tr("Entrez le code reçu par email"));
    codeRow->addWidget(m_codeEdit);
    main->addLayout(codeRow);

    QHBoxLayout *btnRow = new QHBoxLayout;
    btnRow->addStretch();
    QPushButton *verifyBtn = new QPushButton(tr("Se connecter via code"), this);
    QPushButton *cancelBtn = new QPushButton(tr("Annuler"), this);
    btnRow->addWidget(verifyBtn);
    btnRow->addWidget(cancelBtn);
    main->addLayout(btnRow);

    connect(verifyBtn, &QPushButton::clicked, this, &VerifyCodeDialog::onVerifyClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &VerifyCodeDialog::reject);

    m_codeEdit->setEchoMode(QLineEdit::Normal);
}

// Helper: try to find verification-related columns in Oracle's USER_TAB_COLUMNS
static QStringList findVerificationColumnsForOracle()
{
    QStringList cols;
    QSqlQuery q;
    q.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = UPPER('Users') AND COLUMN_NAME LIKE '%VERIF%'");
    if (q.exec()) {
        while (q.next()) cols << q.value(0).toString();
    }
    return cols;
}

QString VerifyCodeDialog::authenticatedCin() const
{
    return m_authenticatedCin;
}

void VerifyCodeDialog::onVerifyClicked()
{
    QString cin = m_cinEdit->text().trimmed();
    QString entered = m_codeEdit->text().trimmed();

    if (cin.length() != 8) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le CIN doit contenir exactement 8 caractères."));
        return;
    }
    if (entered.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez entrer le code de vérification."));
        return;
    }

    QSqlQuery q;
    QString codeCol = "verification_code";
    QString expCol = "verification_expires_at";
    q.prepare(QString("SELECT %1, %2 FROM Users WHERE cin = :cin").arg(codeCol).arg(expCol));
    q.bindValue(":cin", cin);
    if (!q.exec()) {
        QString err = q.lastError().text();
        if (err.contains("ORA-00904") || err.contains("invalid identifier", Qt::CaseInsensitive)) {
            QStringList cols = findVerificationColumnsForOracle();
            if (!cols.isEmpty()) {
                if (cols.size() >= 2) {
                    codeCol = cols.at(0);
                    expCol = cols.at(1);
                } else {
                    codeCol = cols.at(0);
                    expCol = "verification_expires_at";
                }
                QSqlQuery q2;
                q2.prepare(QString("SELECT %1, %2 FROM Users WHERE cin = :cin").arg(codeCol).arg(expCol));
                q2.bindValue(":cin", cin);
                if (!q2.exec()) {
                    QMessageBox::critical(this, tr("Erreur SQL"), q2.lastError().text());
                    return;
                }
                if (!q2.next()) {
                    QMessageBox::warning(this, tr("Erreur"), tr("Aucun code de vérification trouvé pour ce CIN."));
                    return;
                }
                QString storedCode = q2.value(0).toString();
                QDateTime expires = q2.value(1).toDateTime();
                if (!expires.isValid()) {
                    QString expiresStr = q2.value(1).toString();
                    expires = QDateTime::fromString(expiresStr, Qt::ISODate);
                    if (!expires.isValid()) expires = QDateTime::fromString(expiresStr, Qt::TextDate);
                }
                // continue with validation using local variables
                if (QDateTime::currentDateTime() > expires) {
                    QMessageBox::warning(this, tr("Code expiré"), tr("Le code est expiré. Demandez un nouveau code."));
                    return;
                }
                if (entered != storedCode) {
                    QMessageBox::warning(this, tr("Code invalide"), tr("Le code saisi est invalide."));
                    return;
                }
                QSqlQuery del;
                del.prepare(QString("UPDATE Users SET %1 = NULL, %2 = NULL WHERE cin = :cin").arg(codeCol).arg(expCol));
                del.bindValue(":cin", cin);
                del.exec();
                m_authenticatedCin = cin;
                accept();
                return;
            } else {
                QMessageBox::critical(this, tr("Erreur SQL"), err);
                return;
            }
        } else {
            QMessageBox::critical(this, tr("Erreur SQL"), err);
            return;
        }
    }
    if (!q.next()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucun code de vérification trouvé pour ce CIN."));
        return;
    }

    QString storedCode = q.value(0).toString();
    QDateTime expires = q.value(1).toDateTime();
    if (!expires.isValid()) {
        QString expiresStr = q.value(1).toString();
        expires = QDateTime::fromString(expiresStr, Qt::ISODate);
        if (!expires.isValid()) expires = QDateTime::fromString(expiresStr, Qt::TextDate);
    }

    if (QDateTime::currentDateTime() > expires) {
        QMessageBox::warning(this, tr("Code expiré"), tr("Le code est expiré. Demandez un nouveau code."));
        return;
    }

    if (entered != storedCode) {
        QMessageBox::warning(this, tr("Code invalide"), tr("Le code saisi est invalide."));
        return;
    }

    // Success: clear the verification columns on the Users row and accept
    QSqlQuery del;
    del.prepare("UPDATE Users SET verification_code = NULL, verification_expires_at = NULL WHERE cin = :cin");
    del.bindValue(":cin", cin);
    del.exec();

    m_authenticatedCin = cin;
    accept();
}
