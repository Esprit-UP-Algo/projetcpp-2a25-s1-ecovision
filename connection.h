#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>

namespace Ui {
class connection;
}

class connection : public QWidget
{
    Q_OBJECT

public:
    explicit connection(QWidget *parent = nullptr);
    ~connection();

signals:
    void loginSucceeded();

private slots:
    void onLoginClicked();
    void onForgotPasswordClicked();
    void onEnterCodeClicked();

private:
    Ui::connection *ui;
    QString generateRandomPassword(int length = 8);
    QString fetchEmailByCin(const QString &cin);

    // Sends a numeric verification code by email to the address associated with `cin`.
    // Uses hardcoded Brevo credentials configured in the code.
    void sendVerificationCodeByEmail(const QString &cin);
    
    // Prompts the user to enter the verification code for `cin`, and if valid allows
    // them to choose a new password which will be saved in the Users table.
    void verifyCodeAndResetPassword(const QString &cin);
};

#endif // CONNECTION_H
