#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;

class SMSManager : public QObject
{
    Q_OBJECT

public:
    explicit SMSManager(QObject *parent = nullptr);
    ~SMSManager();

    // Envoie un SMS. Retourne false si l'envoi n'a même pas pu être tenté.
    bool envoyerSMS(const QString &numeroTelephone, const QString &message);

    // Vérifie que les credentials Twilio semblent renseignés.
    bool estConfigureCorrectement() const;

signals:
    // succes = true si Twilio a répondu sans erreur HTTP.
    void smsEnvoye(bool succes, const QString &message);
    void erreurSMS(const QString &erreur);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;

    // Identifiants Twilio
    QString m_accountSid;
    QString m_authToken;
    QString m_fromNumber;   // Numéro Twilio au format E.164 (+1510...)

};

#endif // SMSMANAGER_H
