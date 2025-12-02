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
    // Tes vrais identifiants Twilio
   // m_accountSid = "ACe95333f80bf90ba07bc67a49745f41f5";//
   // m_authToken  = "e542ca21165558fc10cea34e37bc4f26";//

    // Numéro Twilio SANS espaces, format E.164 :
    // +1 510 544 8153  ->  +15105448153
   // m_fromNumber = "+15105448153";//

    connect(networkManager,
            QOverload<QNetworkReply *>::of(&QNetworkAccessManager::finished),
            this,
            &SMSManager::onReplyFinished);
}

SMSManager::~SMSManager()
{
    // Rien de spécial, networkManager est enfant de this
}

bool SMSManager::estConfigureCorrectement() const
{
    return !m_accountSid.isEmpty()
    && !m_authToken.isEmpty()
        && !m_fromNumber.isEmpty()
        && !m_accountSid.contains("xxxx", Qt::CaseInsensitive)
        && !m_authToken.contains("your_", Qt::CaseInsensitive);
}

bool SMSManager::envoyerSMS(const QString &numeroTelephone, const QString &message)
{
    // ========== VALIDATION ==========
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
            "Mets ton Account SID, Auth Token et numéro Twilio dans SMSManager.";
        emit erreurSMS(err);
        emit smsEnvoye(false, err);
        qWarning() << "❌ Twilio credentials not configured!";
        return false;
    }

    // ========== FORMATER LE NUMÉRO (Tunisie) ==========
    QString numero = numeroTelephone.trimmed();

    // ex: 93341398 -> +21693341398
    if (numero.startsWith("0") && !numero.startsWith("00")) {
        numero = "+216" + numero.mid(1);
    } else if (numero.startsWith("00216")) {
        numero = "+" + numero.mid(2);      // 00216xxxx -> +216xxxx
    } else if (!numero.startsWith("+")) {
        // par défaut, on suppose Tunisie
        numero = "+216" + numero;
    }

    qDebug() << "📱 Numéro formaté:" << numero;

    // ========== URL TWILIO ==========
    QString urlString =
        QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json")
            .arg(m_accountSid);

    QUrl url(urlString);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/x-www-form-urlencoded");

    // ========== AUTH BASIC ==========
    QString auth = m_accountSid + ":" + m_authToken;
    QByteArray headerData = "Basic " + auth.toLocal8Bit().toBase64();
    request.setRawHeader("Authorization", headerData);

    qDebug() << "🔐 Auth configured for Twilio";

    // ========== CORPS DE LA REQUÊTE ==========
    QUrlQuery params;
    params.addQueryItem("From", m_fromNumber);
    params.addQueryItem("To", numero);
    params.addQueryItem("Body", message);

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    qDebug() << "📤 Envoi SMS :";
    qDebug() << "   From:" << m_fromNumber;
    qDebug() << "   To  :" << numero;
    qDebug() << "   Body:" << message.left(80);

    // ========== ENVOI ==========
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
