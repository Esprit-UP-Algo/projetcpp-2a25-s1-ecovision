#ifndef BREVO_SENDER_H
#define BREVO_SENDER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class BrevoSender : public QObject {
    Q_OBJECT
public:
    explicit BrevoSender(const QString &apiKey, const QString &senderEmail, QObject *parent = nullptr);

    // Sends an email asynchronously. Emits sendFinished when done.
    void sendEmail(const QString &toEmail, const QString &subject, const QString &htmlBody);

signals:
    void sendFinished(bool success, const QString &message);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
};

#endif // BREVO_SENDER_H
