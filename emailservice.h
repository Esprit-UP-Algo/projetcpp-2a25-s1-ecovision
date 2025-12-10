#ifndef EMAILSERVICE_H
#define EMAILSERVICE_H

#include <QObject>
#include <QString>

class EmailService : public QObject
{
    Q_OBJECT

public:
    static EmailService& getInstance();

    // Configuration
    void configureGmail(const QString &email, const QString &appPassword);
    bool isConfigured() const { return m_configured; }

    // Email sending
    bool sendWelcomeEmail(const QString &toEmail, const QString &residentName);
    bool sendEmail(const QString &toEmail, const QString &subject, const QString &body);

private:
    EmailService() = default;
    ~EmailService() = default;

    QString m_smtpServer = "smtp.gmail.com";
    int m_smtpPort = 587;
    QString m_email;
    QString m_appPassword;
    bool m_configured = false;
};

#endif // EMAILSERVICE_H
