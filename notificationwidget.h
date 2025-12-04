#ifndef NOTIFICATIONWIDGET_H
#define NOTIFICATIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPushButton>

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

    // Affiche la notification en bas à droite de l'écran
    void show();  // NE PAS mettre override, show() n’est pas virtuel dans QWidget

signals:
    void closed();

private slots:
    void closeNotification();   // <== slot déclaré ici

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
