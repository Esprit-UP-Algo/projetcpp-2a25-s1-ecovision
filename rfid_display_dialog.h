#ifndef RFID_DISPLAY_DIALOG_H
#define RFID_DISPLAY_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class RfidDisplayDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RfidDisplayDialog(QWidget *parent = nullptr);
    void showRfidUid(const QString &uid);

private:
    QLabel *m_titleLabel;
    QLabel *m_uidLabel;
    QPushButton *m_closeButton;
};

#endif // RFID_DISPLAY_DIALOG_H
