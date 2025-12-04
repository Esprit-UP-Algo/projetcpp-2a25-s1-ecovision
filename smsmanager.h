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

    // Envoie un SMS générique (message déjà construit)
    bool envoyerSMS(const QString &numeroTelephone, const QString &message);

    // Vérifie que les credentials Twilio semblent renseignés
    bool estConfigureCorrectement() const;

    // Construit le texte du SMS de réclamation
    // Bonjour NOM, votre réclamation avec le code CODE a été enregistrée avec succès.
    // + description (si fournie) + délai (si fourni)
    QString construireMessageReclamation(const QString &nomResident,
                                         const QString &codeReclamation,
                                         const QString &description,
                                         const QString &delai) const;

    // Construit le message de réclamation et l’envoie
    bool envoyerSMSReclamation(const QString &numeroTelephone,
                               const QString &nomResident,
                               const QString &codeReclamation,
                               const QString &description,
                               const QString &delai);

signals:
    // succes = true si Twilio a répondu sans erreur HTTP
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
