#include "verification_manager.h"
#include "brevo_sender.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>
#include <QRandomGenerator>
#include "mainwindow.h"

VerificationManager::VerificationManager(QObject *parent)
    : QObject(parent), m_sender(nullptr)
{
}

static QString generateNumericCode(int length = 6)
{
    const QString chars = "0123456789";
    QString code;
    for (int i = 0; i < length; ++i) {
        int idx = QRandomGenerator::global()->bounded(chars.size());
        code.append(chars.at(idx));
    }
    return code;
}

bool VerificationManager::sendVerificationToEmail(const QString &email, const QString &userCin)
{
    if (email.isEmpty() || userCin.isEmpty()) {
        qWarning() << "VerificationManager: email or userCin empty";
        return false;
    }

    // Générer un code à 6 chiffres
    QString code = generateNumericCode(6);
    QDateTime expires = QDateTime::currentDateTimeUtc().addSecs(60 * 60); // 1 heure

    // Insérer ou mettre à jour la table EMAIL_VERIFICATION
    QSqlQuery q;
    // Vérifier si l'email existe
    q.prepare("SELECT email FROM EMAIL_VERIFICATION WHERE email = :email");
    q.bindValue(":email", email);
    if (!q.exec()) {
        qDebug() << "SQL error (select):" << q.lastError().text();
        return false;
    }

    bool exists = q.next();
    if (exists) {
        QSqlQuery q2;
        q2.prepare("UPDATE EMAIL_VERIFICATION SET verification_code = :code, verification_expires_at = :expires WHERE email = :email");
        q2.bindValue(":code", code);
        q2.bindValue(":expires", expires);
        q2.bindValue(":email", email);
        if (!q2.exec()) {
            qDebug() << "SQL error (update):" << q2.lastError().text();
            return false;
        }
    } else {
        QSqlQuery q3;
        q3.prepare("INSERT INTO EMAIL_VERIFICATION (email, verification_code, verification_expires_at) VALUES (:email, :code, :expires)");
        q3.bindValue(":email", email);
        q3.bindValue(":code", code);
        q3.bindValue(":expires", expires);
        if (!q3.exec()) {
            qDebug() << "SQL error (insert):" << q3.lastError().text();
            return false;
        }
    }

    // Préparer l'envoi par Brevo
    // NOTE: le driver Brevo actuel a une clé et sender codés en dur; on passe des placeholders.
    if (m_sender) {
        m_sender->deleteLater();
        m_sender = nullptr;
    }
    // Remplacez les paramètres ci-dessous si vous voulez utiliser d'autres valeurs.
    m_sender = new BrevoSender("<API_KEY>", "amineshimi90@gmail.com", this);
    connect(m_sender, &BrevoSender::sendFinished, this, &VerificationManager::onBrevoSendFinished);

    QString subject = "Code de vérification";
    QString html = QString("<p>Bonjour,</p><p>Votre code de vérification est : <b>%1</b></p><p>Il expirera dans 1 heure.</p>")
                   .arg(code);

    m_sender->sendEmail(email, subject, html);

    return true;
}

void VerificationManager::onBrevoSendFinished(bool success, const QString &message)
{
    emit sendFinished(success, message);
}
