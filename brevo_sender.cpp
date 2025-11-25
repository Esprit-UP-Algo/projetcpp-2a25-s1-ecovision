#include "brevo_sender.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QTimer>

BrevoSender::BrevoSender(const QString &apiKey, const QString &senderEmail, QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)), m_apiKey(apiKey), m_senderEmail(senderEmail)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &BrevoSender::onReplyFinished);
}

void BrevoSender::sendEmail(const QString &toEmail, const QString &subject, const QString &htmlBody)
{
    QUrl url("https://api.brevo.com/v3/smtp/email");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("api-key", m_apiKey.toUtf8());

    QJsonObject root;
    root["sender"] = QJsonObject{{"email", m_senderEmail}};
    root["to"] = QJsonArray{ QJsonObject{{"email", toEmail}} };
    root["subject"] = subject;
    root["htmlContent"] = htmlBody;

    QJsonDocument doc(root);
    QByteArray payload = doc.toJson();

    // POST asynchronously
    m_manager->post(request, payload);
}

void BrevoSender::onReplyFinished(QNetworkReply *reply)
{
    if (!reply) return;

    QByteArray resp = reply->readAll();
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() != QNetworkReply::NoError) {
        QString msg = QString("Network error: %1").arg(reply->errorString());
        emit sendFinished(false, msg);
    } else if (httpStatus >= 200 && httpStatus < 300) {
        emit sendFinished(true, QString::fromUtf8(resp));
    } else {
        QString msg = QString("HTTP %1: %2").arg(httpStatus).arg(QString::fromUtf8(resp));
        emit sendFinished(false, msg);
    }

    reply->deleteLater();
}
