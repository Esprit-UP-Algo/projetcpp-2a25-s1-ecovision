/********************************************************************************
** Form generated from reading UI file 'connection.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTION_H
#define UI_CONNECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_connection
{
public:
    QPushButton *mdpo;
    QLabel *image;
    QLineEdit *CIN_2;
    QLabel *CIN;
    QLabel *mdp;
    QPushButton *pushButton_2;
    QLineEdit *mdp_2;

    void setupUi(QWidget *connection)
    {
        if (connection->objectName().isEmpty())
            connection->setObjectName("connection");
        connection->resize(483, 407);
        mdpo = new QPushButton(connection);
        mdpo->setObjectName("mdpo");
        mdpo->setGeometry(QRect(270, 330, 90, 29));
        mdpo->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
"QPushButton {\n"
"    background-color: #ffffff;\n"
"    color: #3C9ED9; /* texte bleu ciel */\n"
"    border: 1px solid rgba(99, 184, 227, 0.9);\n"
"    border-radius: 5px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* Survol en \303\251tat non s\303\251lectionn\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #F2F9FE; /* l\303\251ger bleu tr\303\250s clair */\n"
"}\n"
"\n"
"/* Appuy\303\251 (momentan\303\251) */\n"
"QPushButton:pressed {\n"
"    background-color: #5CAFE3;\n"
"    color: white;\n"
"}\n"
"\n"
"/* \303\211tat s\303\251lectionn\303\251 (si setCheckable(true)) */\n"
"QPushButton:checked {\n"
"    background-color: #87CEEB; /* bleu ciel */\n"
"    color: white;\n"
"    border-color: #5CAFE3;\n"
"}\n"
"\n"
"/* Survol quand d\303\251j\303\240 s\303\251lectionn\303\251 */\n"
"QPushButton:checked:hover {\n"
"    background-color: #5CAFE3; /* un peu plus fonc\303\251 au survol */\n"
"    color: white;\n"
"}\n"
""));
        image = new QLabel(connection);
        image->setObjectName("image");
        image->setGeometry(QRect(140, 10, 301, 211));
        CIN_2 = new QLineEdit(connection);
        CIN_2->setObjectName("CIN_2");
        CIN_2->setGeometry(QRect(130, 200, 311, 28));
        CIN = new QLabel(connection);
        CIN->setObjectName("CIN");
        CIN->setGeometry(QRect(10, 200, 63, 20));
        mdp = new QLabel(connection);
        mdp->setObjectName("mdp");
        mdp->setGeometry(QRect(10, 240, 141, 20));
        pushButton_2 = new QPushButton(connection);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(100, 330, 90, 29));
        pushButton_2->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
"QPushButton {\n"
"    background-color: #ffffff;\n"
"    color: #3C9ED9; /* texte bleu ciel */\n"
"    border: 1px solid rgba(99, 184, 227, 0.9);\n"
"    border-radius: 5px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* Survol en \303\251tat non s\303\251lectionn\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #F2F9FE; /* l\303\251ger bleu tr\303\250s clair */\n"
"}\n"
"\n"
"/* Appuy\303\251 (momentan\303\251) */\n"
"QPushButton:pressed {\n"
"    background-color: #5CAFE3;\n"
"    color: white;\n"
"}\n"
"\n"
"/* \303\211tat s\303\251lectionn\303\251 (si setCheckable(true)) */\n"
"QPushButton:checked {\n"
"    background-color: #87CEEB; /* bleu ciel */\n"
"    color: white;\n"
"    border-color: #5CAFE3;\n"
"}\n"
"\n"
"/* Survol quand d\303\251j\303\240 s\303\251lectionn\303\251 */\n"
"QPushButton:checked:hover {\n"
"    background-color: #5CAFE3; /* un peu plus fonc\303\251 au survol */\n"
"    color: white;\n"
"}\n"
""));
        mdp_2 = new QLineEdit(connection);
        mdp_2->setObjectName("mdp_2");
        mdp_2->setGeometry(QRect(130, 230, 311, 28));

        retranslateUi(connection);

        QMetaObject::connectSlotsByName(connection);
    } // setupUi

    void retranslateUi(QWidget *connection)
    {
        connection->setWindowTitle(QCoreApplication::translate("connection", "Form", nullptr));
        mdpo->setText(QCoreApplication::translate("connection", "mdp oubliee", nullptr));
        image->setText(QString());
        CIN->setText(QCoreApplication::translate("connection", "Cin :", nullptr));
        mdp->setText(QCoreApplication::translate("connection", "mot de passse :", nullptr));
        pushButton_2->setText(QCoreApplication::translate("connection", "login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class connection: public Ui_connection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTION_H
