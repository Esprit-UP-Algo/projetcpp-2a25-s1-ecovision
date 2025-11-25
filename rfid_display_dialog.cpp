#include "rfid_display_dialog.h"
#include <QFont>
#include <QDateTime>

RfidDisplayDialog::RfidDisplayDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Carte RFID Détectée");
    setMinimumSize(400, 200);
    setModal(false); // Non bloquant

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    // Titre
    m_titleLabel = new QLabel("🔍 Carte RFID Scannée", this);
    QFont titleFont;
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #2e86ab; padding: 10px;");
    layout->addWidget(m_titleLabel);

    // Label pour afficher l'UID
    m_uidLabel = new QLabel("UID: ", this);
    QFont uidFont;
    uidFont.setPointSize(16);
    uidFont.setBold(true);
    uidFont.setFamily("Courier New");
    m_uidLabel->setFont(uidFont);
    m_uidLabel->setAlignment(Qt::AlignCenter);
    m_uidLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #f0f8ff;"
        "   border: 2px solid #4A90E2;"
        "   border-radius: 10px;"
        "   padding: 20px;"
        "   color: #1a5490;"
        "}"
    );
    m_uidLabel->setWordWrap(true);
    layout->addWidget(m_uidLabel);

    // Bouton fermer
    m_closeButton = new QPushButton("Fermer", this);
    m_closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4A90E2;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px 30px;"
        "   font-size: 12px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #357ABD;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2e6da4;"
        "}"
    );
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(m_closeButton, 0, Qt::AlignCenter);

    layout->addStretch();
}

void RfidDisplayDialog::showRfidUid(const QString &uid)
{
    QString timestamp = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    m_uidLabel->setText(QString("UID: %1\n\nDétecté à: %2").arg(uid, timestamp));
    
    // Afficher le dialog
    show();
    raise();
    activateWindow();
}
