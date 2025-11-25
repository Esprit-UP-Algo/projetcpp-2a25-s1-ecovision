#ifndef VERIFICATION_MANAGER_H
#define VERIFICATION_MANAGER_H

#include <QObject>
#include <QString>

class BrevoSender;

class VerificationManager : public QObject {
    Q_OBJECT
public:
    explicit VerificationManager(QObject *parent = nullptr);

    // Génère un code, l'enregistre en base et demande l'envoi via Brevo
    // Retourne true si l'opération d'écriture en base a été lancée correctement
    bool sendVerificationToEmail(const QString &email, const QString &userCin);

signals:
    void sendFinished(bool success, const QString &message);

private slots:
    void onBrevoSendFinished(bool success, const QString &message);

private:
    BrevoSender *m_sender;
};

#endif // VERIFICATION_MANAGER_H
