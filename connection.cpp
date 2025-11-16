// connection.cpp
#include "connection.h"
#include "ui_connection.h"
#include "brevo_sender.h"
#include "verifycodedialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QSqlRecord>
#include <QDateTime>
#include <QByteArray>
#include <QDebug>

// Helper: try to find verification-related columns in Oracle's USER_TAB_COLUMNS
static QStringList findVerificationColumnsForOracle()
{
    QStringList cols;
    QSqlQuery q;
    // This query works for Oracle (USER_TAB_COLUMNS). Table names are uppercased by default.
    q.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = UPPER('Users') AND COLUMN_NAME LIKE '%VERIF%'");
    if (q.exec()) {
        while (q.next()) cols << q.value(0).toString();
    }
    return cols;
}

connection::connection(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::connection)
{
    ui->setupUi(this);

    // Initial-phase behavior: no automatic password emailing by default.
    connect(ui->pushButton_2, &QPushButton::clicked, this, &connection::onLoginClicked);
    connect(ui->mdpo, &QPushButton::clicked, this, &connection::onForgotPasswordClicked);

    // Add a small "I have a code" button so user can manually enter a received code
    QPushButton *enterCodeBtn = new QPushButton(tr("J'ai un code"), this);
    enterCodeBtn->setGeometry(370, 330, 90, 29);
    connect(enterCodeBtn, &QPushButton::clicked, this, &connection::onEnterCodeClicked);
}

connection::~connection()
{
    delete ui;
}

void connection::onLoginClicked()
{
    const QString cin = ui->CIN_2->text().trimmed();
    const QString mdp = ui->mdp_2->text().trimmed();

    if (cin.length() != 8) {
        QMessageBox::warning(this, tr("Connexion"), tr("CIN invalide (8 caractères)."));
        return;
    }
    if (mdp.isEmpty()) {
        QMessageBox::warning(this, tr("Connexion"), tr("Mot de passe requis."));
        return;
    }

    QSqlQuery q;
    q.prepare("SELECT 1 FROM Users WHERE cin = :cin AND mdp = :mdp");
    q.bindValue(":cin", cin);
    q.bindValue(":mdp", mdp);
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Erreur SQL"), q.lastError().text());
        return;
    }
    if (q.next()) {
        emit loginSucceeded();
    } else {
        QMessageBox::warning(this, tr("Connexion"), tr("CIN ou mot de passe invalide."));
    }
}

QString connection::generateRandomPassword(int length)
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    QString password;

    for (int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        password.append(chars[index]);
    }

    return password;
}

static QString generateNumericCode(int length = 6)
{
    QString code;
    for (int i = 0; i < length; ++i) {
        int d = QRandomGenerator::global()->bounded(10);
        code.append(QChar('0' + d));
    }
    return code;
}

void connection::sendVerificationCodeByEmail(const QString &cin)
{
    QString email = fetchEmailByCin(cin);
    if (email == "__NOT_FOUND__") {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucun compte trouvé avec ce CIN."));
        return;
    }

    // Generate code and expiry
    QString code = generateNumericCode(6);
    QDateTime expires = QDateTime::currentDateTime().addSecs(10 * 60); // 10 minutes

    // Ensure Users table exists: we will store the verification code on the Users row
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.tables().contains("Users", Qt::CaseInsensitive)) {
        QMessageBox::critical(this, tr("Table manquante"), tr("La table 'Users' est manquante. Vérifiez votre schéma dans la base."));
        return;
    }

    // Store code and expiry on the Users table. Use UPDATE so we don't rely on
    // creating separate tables which can be fragile across DB drivers.
    QSqlQuery updQ;
    QString codeCol = "verification_code";
    QString expCol = "verification_expires_at";
    updQ.prepare(QString("UPDATE Users SET %1 = :code, %2 = :expires WHERE cin = :cin").arg(codeCol).arg(expCol));
    updQ.bindValue(":code", code);
    // Bind QDateTime directly so the driver can convert to the database TIMESTAMP type
    updQ.bindValue(":expires", expires);
    updQ.bindValue(":cin", cin);
    if (!updQ.exec()) {
        QString err = updQ.lastError().text();
        // If Oracle signals invalid identifier (missing column), try to detect actual column names
        if (err.contains("ORA-00904") || err.contains("invalid identifier", Qt::CaseInsensitive)) {
            QStringList cols = findVerificationColumnsForOracle();
            if (!cols.isEmpty()) {
                // Attempt to pick two columns: first for code, second for expires (best-effort)
                if (cols.size() >= 2) {
                    codeCol = cols.at(0);
                    expCol = cols.at(1);
                } else {
                    codeCol = cols.at(0);
                    // guess expiry column name
                    expCol = "verification_expires_at";
                }
                QSqlQuery updQ2;
                updQ2.prepare(QString("UPDATE Users SET %1 = :code, %2 = :expires WHERE cin = :cin").arg(codeCol).arg(expCol));
                updQ2.bindValue(":code", code);
                updQ2.bindValue(":expires", expires);
                updQ2.bindValue(":cin", cin);
                if (!updQ2.exec()) {
                    QMessageBox::critical(this, tr("Erreur SQL"), updQ2.lastError().text());
                    return;
                }
                if (updQ2.numRowsAffected() == 0) {
                    QMessageBox::warning(this, tr("Aucun compte"), tr("Aucun compte trouvé avec ce CIN. Vérifiez le CIN fourni."));
                    return;
                }
            } else {
                QMessageBox::critical(this, tr("Erreur SQL"), err);
                return;
            }
        } else {
            QMessageBox::critical(this, tr("Erreur SQL"), err);
            return;
        }
    } else {
        // executed OK; ensure a row was updated
        if (updQ.numRowsAffected() == 0) {
            QMessageBox::warning(this, tr("Aucun compte"), tr("Aucun compte trouvé avec ce CIN. Vérifiez le CIN fourni."));
            return;
        }
    }

    // Hardcoded Brevo credentials (already configured in brevo_sender.cpp)
    QString apiKey = "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";
    QString senderEmail = "amineshimi90@gmail.com";

    BrevoSender *sender = new BrevoSender(apiKey, senderEmail, this);
    connect(sender, &BrevoSender::sendFinished, this, [this, code, cin](bool success, const QString &message){
        if (success) {
            QMessageBox::information(this, tr("Code envoyé"), tr("Le code de vérification a été envoyé par email."));
            // After successful send, open the verification dialog so the user can enter the code and login
            VerifyCodeDialog dlg(cin, this);
            if (dlg.exec() == QDialog::Accepted) {
                // authenticated by code
                emit loginSucceeded();
            }
        } else {
            QMessageBox::warning(this, tr("Échec envoi"), tr("Impossible d'envoyer le code: %1").arg(message));
        }
    });

    QString subject = tr("Code de vérification");
    QString html = tr("<p>Votre code de vérification est : <b>%1</b></p><p>Valide pendant 10 minutes.</p>").arg(code);
    sender->sendEmail(email, subject, html);
}

void connection::onForgotPasswordClicked()
{
    // Demander le CIN à l'utilisateur
    bool ok;
    QString cin = QInputDialog::getText(this, tr("Mot de passe oublié"),
                                         tr("Entrez votre CIN (8 chiffres):"),
                                         QLineEdit::Normal, "", &ok);

    if (!ok || cin.isEmpty()) {
        return;
    }

    cin = cin.trimmed();

    if (cin.length() != 8) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le CIN doit contenir exactement 8 caractères."));
        return;
    }

    // Try to send verification code by email; this function will warn if no email or config
    sendVerificationCodeByEmail(cin);
}

void connection::verifyCodeAndResetPassword(const QString &cin)
{
    // Prompt the user to enter the code
    bool ok;
    QString entered = QInputDialog::getText(this, tr("Entrer le code"),
                                            tr("Entrez le code de vérification reçu par email:"),
                                            QLineEdit::Normal, QString(), &ok);
    if (!ok) return;
    entered = entered.trimmed();

    if (entered.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Code vide."));
        return;
    }

    QSqlQuery q;
    q.prepare("SELECT verification_code, verification_expires_at FROM Users WHERE cin = :cin");
    q.bindValue(":cin", cin);
    if (!q.exec()) {
        QMessageBox::critical(this, tr("Erreur SQL"), q.lastError().text());
        return;
    }
    if (!q.next()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Aucun code de vérification trouvé pour ce CIN."));
        return;
    }

    QString storedCode = q.value(0).toString();
    // Try to get a QDateTime directly; fallback to parsing string if needed
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

    // Prompt for new password (and confirmation)
    QString newPwd = QInputDialog::getText(this, tr("Nouveau mot de passe"),
                                           tr("Entrez le nouveau mot de passe:"),
                                           QLineEdit::Password, QString(), &ok);
    if (!ok) return;
    QString confirm = QInputDialog::getText(this, tr("Confirmer mot de passe"),
                                            tr("Confirmez le nouveau mot de passe:"),
                                            QLineEdit::Password, QString(), &ok);
    if (!ok) return;
    if (newPwd != confirm) {
        QMessageBox::warning(this, tr("Erreur"), tr("Les mots de passe ne correspondent pas."));
        return;
    }

    // Update password in Users table
    QSqlQuery upd;
    upd.prepare("UPDATE Users SET mdp = :mdp WHERE cin = :cin");
    upd.bindValue(":mdp", newPwd);
    upd.bindValue(":cin", cin);
    if (!upd.exec()) {
        QMessageBox::critical(this, tr("Erreur SQL"), upd.lastError().text());
        return;
    }

    // Clear verification code fields on the Users row
    QSqlQuery del;
    del.prepare("UPDATE Users SET verification_code = NULL, verification_expires_at = NULL WHERE cin = :cin");
    del.bindValue(":cin", cin);
    del.exec();

    QMessageBox::information(this, tr("Mot de passe changé"), tr("Votre mot de passe a été mis à jour avec succès."));
}

void connection::onEnterCodeClicked()
{
    bool ok;
    QString cin = QInputDialog::getText(this, tr("Entrer CIN"),
                                        tr("Entrez votre CIN (8 chiffres):"),
                                        QLineEdit::Normal, QString(), &ok);
    if (!ok || cin.isEmpty()) return;
    cin = cin.trimmed();
    if (cin.length() != 8) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le CIN doit contenir exactement 8 caractères."));
        return;
    }

    verifyCodeAndResetPassword(cin);
}

QString connection::fetchEmailByCin(const QString &cin)
{
    // Essai table EMAIL
    QSqlQuery qEmail;
    qEmail.prepare("SELECT email FROM EMAIL WHERE cin = :cin");
    qEmail.bindValue(":cin", cin);
    if (qEmail.exec() && qEmail.next()) {
        return qEmail.value(0).toString().trimmed();
    }

    // Fallback sur table Users si elle existe encore pour compatibilité
    QSqlQuery qUsers;
    qUsers.prepare("SELECT email FROM Users WHERE cin = :cin");
    qUsers.bindValue(":cin", cin);
    if (qUsers.exec() && qUsers.next()) {
        return qUsers.value(0).toString().trimmed();
    }

    // Si aucune correspondance
    return "__NOT_FOUND__";
}

