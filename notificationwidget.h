#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class NotificationWidget : public QWidget
{
    Q_OBJECT

public:
    enum NotificationType {
        Success,    // Vert
        Warning,    // Orange
        Error,      // Rouge
        Info        // Bleu
    };

    explicit NotificationWidget(const QString &title,
                                const QString &message,
                                NotificationType type = Info,
                                QWidget *parent = nullptr);
    ~NotificationWidget();

    void show();  // on enlève "override"

signals:
    void closed();

private slots:
    void closeNotification();

private:
    QLabel *titleLabel;
    QLabel *messageLabel;
    QPushButton *closeButton;
    QPropertyAnimation *showAnimation;
    QPropertyAnimation *hideAnimation;
    QTimer *autoCloseTimer;

    void setupUI(NotificationType type);
    QString getColorForType(NotificationType type);
    QString getIconForType(NotificationType type);
};

#endif // NOTIFICATIONWIDGET_H
