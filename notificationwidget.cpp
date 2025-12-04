#include "notificationwidget.h"

#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QScreen>

NotificationWidget::NotificationWidget(const QString &title,
                                       const QString &message,
                                       NotificationType type,
                                       QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    messageLabel(nullptr),
    closeButton(nullptr),
    showAnimation(nullptr),
    hideAnimation(nullptr),
    autoCloseTimer(nullptr)
{
    setupUI(type);

    titleLabel->setText(getIconForType(type) + " " + title);
    messageLabel->setText(message);

    autoCloseTimer = new QTimer(this);
    autoCloseTimer->setSingleShot(true);
    connect(autoCloseTimer, &QTimer::timeout,
            this, &NotificationWidget::closeNotification);
}

NotificationWidget::~NotificationWidget()
{
}

void NotificationWidget::setupUI(NotificationType type)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setFixedSize(350, 100);

    QWidget *mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    mainWidget->setGeometry(0, 0, 350, 100);

    QString bgColor = getColorForType(type);
    mainWidget->setStyleSheet(QString(
                                  "#mainWidget {"
                                  "    background-color: %1;"
                                  "    border-radius: 10px;"
                                  "    border: 2px solid rgba(255, 255, 255, 0.3);"
                                  "}"
                                  ).arg(bgColor));

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(15, 10, 15, 10);

    QHBoxLayout *headerLayout = new QHBoxLayout();

    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        );

    closeButton = new QPushButton("✕", this);
    closeButton->setFixedSize(25, 25);
    closeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0.2);"
        "    border: none;"
        "    border-radius: 12px;"
        "    color: white;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.4);"
        "}"
        );
    connect(closeButton, &QPushButton::clicked,
            this, &NotificationWidget::closeNotification);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(closeButton);

    messageLabel = new QLabel(this);
    messageLabel->setWordWrap(true);
    messageLabel->setStyleSheet(
        "QLabel {"
        "    color: rgba(255, 255, 255, 0.9);"
        "    font-size: 12px;"
        "}"
        );

    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(messageLabel);
    mainLayout->addStretch();

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(opacityEffect);

    showAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    showAnimation->setDuration(300);
    showAnimation->setStartValue(0.0);
    showAnimation->setEndValue(1.0);

    hideAnimation = new QPropertyAnimation(opacityEffect, "opacity", this);
    hideAnimation->setDuration(300);
    hideAnimation->setStartValue(1.0);
    hideAnimation->setEndValue(0.0);
    connect(hideAnimation, &QPropertyAnimation::finished,
            this, &NotificationWidget::close);
}

QString NotificationWidget::getColorForType(NotificationType type)
{
    switch (type) {
    case Success:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #11998e, stop:1 #38ef7d)";
    case Warning:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f2994a, stop:1 #f2c94c)";
    case Error:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eb3349, stop:1 #f45c43)";
    case Info:
    default:
        return "qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #667eea, stop:1 #764ba2)";
    }
}

QString NotificationWidget::getIconForType(NotificationType type)
{
    switch (type) {
    case Success: return "✅";
    case Warning: return "⚠️";
    case Error:   return "❌";
    case Info:
    default:      return "ℹ️";
    }
}

void NotificationWidget::show()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int margin = 20;

        int x = screenGeometry.right() - width() - margin;
        int y = screenGeometry.bottom() - height() - margin;

        move(x, y);
    }

    QWidget::show();
    raise();
    activateWindow();

    if (showAnimation)
        showAnimation->start();

    if (autoCloseTimer)
        autoCloseTimer->start(5000);
}

void NotificationWidget::closeNotification()
{
    if (autoCloseTimer)
        autoCloseTimer->stop();

    if (hideAnimation)
        hideAnimation->start();
    else
        close();

    emit closed();
}
