/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *groupBox;
    QPushButton *pushButton_2_ressources;
    QPushButton *pushButton_3_ressources;
    QPushButton *pushButton_4_ressources;
    QPushButton *pushButton_ressources;
    QPushButton *pushButton_5_ressources;
    QLabel *label_9;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1274, 747);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(0, 20, 331, 671));
        groupBox->setStyleSheet(QString::fromUtf8("background-color: #5cb5fe;"));
        pushButton_2_ressources = new QPushButton(groupBox);
        pushButton_2_ressources->setObjectName("pushButton_2_ressources");
        pushButton_2_ressources->setGeometry(QRect(80, 280, 151, 41));
        pushButton_2_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_2_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;              /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(255, 255, 255, 90);\n"
"    background-color: #5cb4f2;        /* couleur par d\303\251faut */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;       /* image plus petite que le bouton */\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover */\n"
"#pushButton_2_ressources:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/"
                        "* Press\303\251 */\n"
"#pushButton_2_ressources:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        pushButton_3_ressources = new QPushButton(groupBox);
        pushButton_3_ressources->setObjectName("pushButton_3_ressources");
        pushButton_3_ressources->setGeometry(QRect(80, 520, 151, 41));
        pushButton_3_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_3_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;              /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(255, 255, 255, 90);\n"
"    background-color: #5cb4f2;        /* couleur par d\303\251faut */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;       /* image plus petite que le bouton */\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover */\n"
"#pushButton_3_ressources:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/"
                        "* Press\303\251 */\n"
"#pushButton_3_ressources:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        pushButton_4_ressources = new QPushButton(groupBox);
        pushButton_4_ressources->setObjectName("pushButton_4_ressources");
        pushButton_4_ressources->setGeometry(QRect(80, 440, 151, 41));
        pushButton_4_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_4_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;              /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(255, 255, 255, 90);\n"
"    background-color: #5cb4f2;        /* couleur par d\303\251faut */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;       /* image plus petite que le bouton */\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover */\n"
"#pushButton_4_ressources:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/"
                        "* Press\303\251 */\n"
"#pushButton_4_ressources:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        pushButton_ressources = new QPushButton(groupBox);
        pushButton_ressources->setObjectName("pushButton_ressources");
        pushButton_ressources->setGeometry(QRect(80, 210, 151, 41));
        pushButton_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;              /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(255, 255, 255, 90);\n"
"    background-color: #5cb4f2;        /* couleur par d\303\251faut */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;       /* image plus petite que le bouton */\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover */\n"
"#pushButton_ressources:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Pr"
                        "ess\303\251 */\n"
"#pushButton_ressources:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        pushButton_5_ressources = new QPushButton(groupBox);
        pushButton_5_ressources->setObjectName("pushButton_5_ressources");
        pushButton_5_ressources->setGeometry(QRect(80, 360, 151, 41));
        pushButton_5_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_5_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;              /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(255, 255, 255, 90);\n"
"    background-color: #5cb4f2;        /* couleur par d\303\251faut */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;       /* image plus petite que le bouton */\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover */\n"
"#pushButton_5_ressources:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/"
                        "* Press\303\251 */\n"
"#pushButton_5_ressources:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 40, 241, 141));
        label_9->setPixmap(QPixmap(QString::fromUtf8("image.png")));
        label_9->setScaledContents(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1274, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QString());
        pushButton_2_ressources->setText(QCoreApplication::translate("MainWindow", "Gestion local ", nullptr));
        pushButton_3_ressources->setText(QCoreApplication::translate("MainWindow", "Gestion resident", nullptr));
        pushButton_4_ressources->setText(QCoreApplication::translate("MainWindow", "Gestion Reclamation", nullptr));
        pushButton_ressources->setText(QCoreApplication::translate("MainWindow", "Gestion emply\303\251\303\251", nullptr));
        pushButton_5_ressources->setText(QCoreApplication::translate("MainWindow", "Gestion Ressources ", nullptr));
        label_9->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
