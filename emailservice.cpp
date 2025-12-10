#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QObject>
#include <QString>
#include <QSettings>

class EmailService : public QObject
{
    Q_OBJECT

public:
    static EmailService& getInstance();
    bool sendWelcomeEmail(const QString &toEmail, const QString &residentName);
    bool sendNotification(const QString &toEmail, const QString &subject, const QString &body);
    void configureSMTP(const QString &smtpServer, int port, const QString &username, const QString &password);

private:
    EmailService() = default;
    ~EmailService() = default;

    QString smtpServer;
    int smtpPort;
    QString username;
    QString password;
    bool isConfigured;
};

#endif // EMAILSERVICE_H
