#ifndef VERIFYCODEDIALOG_H
#define VERIFYCODEDIALOG_H

#include <QDialog>

class QLineEdit;

class VerifyCodeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VerifyCodeDialog(const QString &prefillCin = QString(), QWidget *parent = nullptr);

    // If accepted, use selected CIN as the authenticated user
    QString authenticatedCin() const;

private slots:
    void onVerifyClicked();

private:
    QLineEdit *m_cinEdit;
    QLineEdit *m_codeEdit;
    QString m_authenticatedCin;
};

#endif // VERIFYCODEDIALOG_H
