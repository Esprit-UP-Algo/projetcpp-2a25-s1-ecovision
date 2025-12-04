#include "smsmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

SMSManager::SMSManager(QObject *parent)
    : QObject(parent),
    networkManager(new QNetworkAccessManager(this))
{
    // Mets ici tes vrais identifiants Twilio
    // Ne laisse pas ça en dur en prod, mais pour tester ça va.
    m_accountSid = "";
    m_authToken  = "";

    // Numéro Twilio EN FORMAT E.164, sans espaces
    // Exemple : "+15105448153"
    m_fromNumber = "";

    connect(networkManager,
            QOverload<QNetworkReply *>::of(&QNetworkAccessManager::finished),
            this,
            &SMSManager::onReplyFinished);
}

SMSManager::~SMSManager()
{
    // networkManager est enfant de this, rien à faire
}

bool SMSManager::estConfigureCorrectement() const
{
    return !m_accountSid.isEmpty()
    && !m_authToken.isEmpty()
        && !m_fromNumber.isEmpty()
        && !m_accountSid.contains("xxxx", Qt::CaseInsensitive)
        && !m_authToken.contains("xxxx", Qt::CaseInsensitive);
}

// ===================================
// Construction du message de réclamation
// ===================================

QString SMSManager::construireMessageReclamation(const QString &nomResident,
                                                 const QString &codeReclamation,
                                                 const QString &description,
                                                 const QString &delai) const
{
    // Message conforme à ce que tu veux :
    // Bonjour [Nom du résident], votre réclamation avec le code [CODE] a été enregistrée avec succès.
    // Description : ...
    // Délai estimé de traitement : ...
    // Merci de votre confiance.

    QString msg = QString("Bonjour %1, ").arg(nomResident);
    msg += QString("votre réclamation avec le code %1 a été enregistrée avec succès. ")
               .arg(codeReclamation);

    if (!description.trimmed().isEmpty()) {
        msg += QString("Description : %1. ").arg(description);
    }

    if (!delai.trimmed().isEmpty()) {
        msg += QString("Délai estimé de traitement : %1. ").arg(delai);
    }

    msg += "Merci de votre confiance.";

    return msg;
}

bool SMSManager::envoyerSMSReclamation(const QString &numeroTelephone,
                                       const QString &nomResident,
                                       const QString &codeReclamation,
                                       const QString &description,
                                       const QString &delai)
{
    QString message = construireMessageReclamation(nomResident,
                                                   codeReclamation,
                                                   description,
                                                   delai);
    return envoyerSMS(numeroTelephone, message);
}

// ===================================
// Envoi générique d’un SMS via Twilio
// ===================================

bool SMSManager::envoyerSMS(const QString &numeroTelephone, const QString &message)
{
    if (numeroTelephone.isEmpty() || message.isEmpty()) {
        const QString err = "Numéro de téléphone ou message vide";
        emit erreurSMS(err);
        emit smsEnvoye(false, err);
        qWarning() << "❌" << err;
        return false;
    }

    if (!estConfigureCorrectement()) {
        const QString err =
            "Configuration Twilio non définie. "
            "Renseigne Account SID, Auth Token et numéro Twilio dans SMSManager.";
        emit erreurSMS(err);
        emit smsEnvoye(false, err);
        qWarning() << "❌ Twilio credentials not configured!";
        return false;
    }

    // Formatage du numéro (Tunisie par défaut)
    QString numero = numeroTelephone.trimmed();

    // ex: 93341398 -> +21693341398
    if (numero.startsWith("0") && !numero.startsWith("00")) {
        numero = "+216" + numero.mid(1);
    } else if (numero.startsWith("00216")) {
        numero = "+" + numero.mid(2);      // 00216xxxx -> +216xxxx
    } else if (!numero.startsWith("+")) {
        numero = "+216" + numero;
    }

    qDebug() << "📱 Numéro formaté:" << numero;

    // URL Twilio
    QString urlString =
        QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json")
            .arg(m_accountSid);

    QUrl url(urlString);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    // Auth basic
    QString auth = m_accountSid + ":" + m_authToken;
    QByteArray headerData = "Basic " + auth.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", headerData);

    qDebug() << "🔐 Auth configured for Twilio";

    // Corps de la requête
    QUrlQuery params;
    params.addQueryItem("From", m_fromNumber);
    params.addQueryItem("To", numero);
    params.addQueryItem("Body", message);

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    qDebug() << "📤 Envoi SMS :";
    qDebug() << "   From:" << m_fromNumber;
    qDebug() << "   To  :" << numero;
    qDebug() << "   Body:" << message.left(160);

    QNetworkReply *reply = networkManager->post(request, postData);

    if (!reply) {
        const QString err = "Erreur: impossible de créer la requête réseau";
        emit erreurSMS(err);
        emit smsEnvoye(false, err);
        qWarning() << "❌" << err;
        return false;
    }

    reply->setProperty("to", numero);
    return true;
}

// ===================================
// Réponse Twilio
// ===================================

void SMSManager::onReplyFinished(QNetworkReply *reply)
{
    if (!reply) {
        const QString err = "Erreur: réponse vide du serveur";
        emit erreurSMS(err);
        emit smsEnvoye(false, err);
        return;
    }

    const QString to = reply->property("to").toString();

    if (reply->error() == QNetworkReply::NoError) {
        QString response = QString::fromUtf8(reply->readAll());

        qDebug() << "📨 Réponse Twilio pour" << to << ":" << response;

        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            const QString messageSid = obj["sid"].toString();
            const QString status     = obj["status"].toString();

            const QString okMsg =
                QString("SMS envoyé avec succès (SID: %1, status: %2)")
                    .arg(messageSid, status);

            qDebug() << "✅" << okMsg;
            emit smsEnvoye(true, okMsg);
        } else {
            const QString okMsg = "SMS envoyé (réponse JSON non reconnue)";
            qDebug() << "⚠️" << okMsg;
            emit smsEnvoye(true, okMsg);
        }
    } else {
        QString errorMsg = reply->errorString();
        QString response = QString::fromUtf8(reply->readAll());

        qDebug() << "❌ Erreur SMS vers" << to << ":" << errorMsg;
        qDebug() << "   Réponse brute:" << response;

        QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("message")) {
                QString errorDetails = obj["message"].toString();
                errorMsg += " - " + errorDetails;
            }
        }

        emit erreurSMS(errorMsg);
        emit smsEnvoye(false, errorMsg);
    }

    reply->deleteLater();
}
