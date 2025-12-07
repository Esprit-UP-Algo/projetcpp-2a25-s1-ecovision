/********************************************************************************
** Form generated from reading UI file 'resourcewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESOURCEWINDOW_H
#define UI_RESOURCEWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *btn_ajouter;
    QLabel *label;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *btn_modifier;
    QPushButton *btn_supprimer;
    QLineEdit *lineEdit_id;
    QLineEdit *lineEdit_localisation;
    QLineEdit *lineEdit_consommation;
    QTableWidget *tableWidget;
    QRadioButton *radio_eau;
    QRadioButton *radio_elec;
    QDateEdit *dateEdit_mesure;
    QPushButton *pushButton_4;
    QPushButton *btn_afficher_id;
    QPushButton *btn_recherche;
    QPushButton *btn_afficher_prix;
    QListWidget *listWidget;
    QPushButton *btn_afficher;
    QPushButton *btn_pdf_avance;
    QPushButton *btn_qrcode;
    QPushButton *statsButton;
    QWidget *tab_2;
    QTableWidget *tableWidget_2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(1198, 982);
        tabWidget = new QTabWidget(Dialog);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(50, 80, 1001, 721));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tab = new QWidget();
        tab->setObjectName("tab");
        btn_ajouter = new QPushButton(tab);
        btn_ajouter->setObjectName("btn_ajouter");
        btn_ajouter->setGeometry(QRect(210, 320, 151, 41));
        btn_ajouter->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_ajouter {\n"
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
"#btn_ajouter:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
"#"
                        "btn_ajouter:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 70, 111, 31));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        label->setFont(font);
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(110, 120, 211, 31));
        label_3->setFont(font);
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 170, 121, 31));
        label_2->setFont(font);
        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(110, 220, 181, 41));
        label_4->setFont(font);
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(110, 270, 161, 41));
        label_5->setFont(font);
        btn_modifier = new QPushButton(tab);
        btn_modifier->setObjectName("btn_modifier");
        btn_modifier->setGeometry(QRect(390, 320, 151, 41));
        btn_modifier->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_modifier {\n"
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
"#btn_modifier:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
""
                        "#btn_modifier:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        btn_supprimer = new QPushButton(tab);
        btn_supprimer->setObjectName("btn_supprimer");
        btn_supprimer->setGeometry(QRect(570, 320, 151, 41));
        btn_supprimer->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_supprimer {\n"
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
"#btn_supprimer:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
""
                        "#btn_supprimer:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        lineEdit_id = new QLineEdit(tab);
        lineEdit_id->setObjectName("lineEdit_id");
        lineEdit_id->setGeometry(QRect(330, 70, 391, 31));
        lineEdit_id->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"\n"
"background-color: white; /* fond blanc */\n"
"\n"
"color: black; /* texte en noir */\n"
"\n"
"border: 2px solid black; /* contour noir */\n"
"\n"
"border-radius: 8px; /* coins arrondis */\n"
"\n"
"padding: 8px 20px; /* espace int\303\251rieur */\n"
"\n"
"font-size: 14px; /* taille du texte */\n"
"\n"
"font-weight: bold; /* texte en gras */\n"
"\n"
"}\n"
""));
        lineEdit_localisation = new QLineEdit(tab);
        lineEdit_localisation->setObjectName("lineEdit_localisation");
        lineEdit_localisation->setGeometry(QRect(330, 170, 391, 31));
        lineEdit_localisation->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"\n"
"background-color: white; /* fond blanc */\n"
"\n"
"color: black; /* texte en noir */\n"
"\n"
"border: 2px solid black; /* contour noir */\n"
"\n"
"border-radius: 8px; /* coins arrondis */\n"
"\n"
"padding: 8px 20px; /* espace int\303\251rieur */\n"
"\n"
"font-size: 14px; /* taille du texte */\n"
"\n"
"font-weight: bold; /* texte en gras */\n"
"\n"
"}"));
        lineEdit_consommation = new QLineEdit(tab);
        lineEdit_consommation->setObjectName("lineEdit_consommation");
        lineEdit_consommation->setGeometry(QRect(330, 220, 391, 31));
        lineEdit_consommation->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"\n"
"background-color: white; /* fond blanc */\n"
"\n"
"color: black; /* texte en noir */\n"
"\n"
"border: 2px solid black; /* contour noir */\n"
"\n"
"border-radius: 8px; /* coins arrondis */\n"
"\n"
"padding: 8px 20px; /* espace int\303\251rieur */\n"
"\n"
"font-size: 14px; /* taille du texte */\n"
"\n"
"font-weight: bold; /* texte en gras */\n"
"\n"
"}"));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 5)
            tableWidget->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(70, 450, 621, 191));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        radio_eau = new QRadioButton(tab);
        radio_eau->setObjectName("radio_eau");
        radio_eau->setGeometry(QRect(369, 123, 131, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        radio_eau->setFont(font1);
        radio_elec = new QRadioButton(tab);
        radio_elec->setObjectName("radio_elec");
        radio_elec->setGeometry(QRect(529, 123, 131, 31));
        radio_elec->setFont(font1);
        dateEdit_mesure = new QDateEdit(tab);
        dateEdit_mesure->setObjectName("dateEdit_mesure");
        dateEdit_mesure->setGeometry(QRect(330, 270, 391, 31));
        QFont font2;
        font2.setPointSize(12);
        dateEdit_mesure->setFont(font2);
        dateEdit_mesure->setDate(QDate(2025, 10, 22));
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(780, 0, 151, 41));
        QFont font3;
        font3.setBold(true);
        pushButton_4->setFont(font3);
        pushButton_4->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#pushButton_4 {\n"
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
"#pushButton_4:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
""
                        "#pushButton_4:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        btn_afficher_id = new QPushButton(tab);
        btn_afficher_id->setObjectName("btn_afficher_id");
        btn_afficher_id->setGeometry(QRect(330, 390, 151, 41));
        btn_afficher_id->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_afficher_id{\n"
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
"#btn_afficher_id:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 *"
                        "/\n"
"#btn_afficher_id:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        btn_recherche = new QPushButton(tab);
        btn_recherche->setObjectName("btn_recherche");
        btn_recherche->setGeometry(QRect(740, 70, 151, 41));
        btn_recherche->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_recherche {\n"
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
"#btn_recherche:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
""
                        "#btn_recherche:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        btn_afficher_prix = new QPushButton(tab);
        btn_afficher_prix->setObjectName("btn_afficher_prix");
        btn_afficher_prix->setGeometry(QRect(720, 380, 151, 41));
        btn_afficher_prix->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_afficher_prix{\n"
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
"#btn_afficher_prix:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251"
                        " */\n"
"#btn_afficher_prix:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        listWidget = new QListWidget(tab);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(710, 450, 256, 192));
        btn_afficher = new QPushButton(tab);
        btn_afficher->setObjectName("btn_afficher");
        btn_afficher->setGeometry(QRect(970, 670, 20, 20));
        btn_pdf_avance = new QPushButton(tab);
        btn_pdf_avance->setObjectName("btn_pdf_avance");
        btn_pdf_avance->setGeometry(QRect(600, 0, 151, 41));
        btn_pdf_avance->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_pdf_avance {\n"
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
"#btn_pdf_avance:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */"
                        "\n"
"#btn_pdf_avance:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        btn_qrcode = new QPushButton(tab);
        btn_qrcode->setObjectName("btn_qrcode");
        btn_qrcode->setGeometry(QRect(840, 180, 131, 41));
        btn_qrcode->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_qrcode {\n"
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
"#btn_qrcode:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
"#bt"
                        "n_qrcode:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}\n"
""));
        statsButton = new QPushButton(tab);
        statsButton->setObjectName("statsButton");
        statsButton->setGeometry(QRect(840, 240, 131, 41));
        statsButton->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#statsButton {\n"
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
"#statsButton:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
"#"
                        "statsButton:pressed {\n"
"    background-color: #2f85c9;\n"
"    color: white;\n"
"    transform: scale(0.95);\n"
"}"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tableWidget_2 = new QTableWidget(tab_2);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(110, 100, 731, 411));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(Dialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        btn_ajouter->setText(QCoreApplication::translate("Dialog", "ajouter", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "code:", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "type :", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "localisation", nullptr));
        label_4->setText(QCoreApplication::translate("Dialog", "consommation :", nullptr));
        label_5->setText(QCoreApplication::translate("Dialog", "date_mesure :", nullptr));
        btn_modifier->setText(QCoreApplication::translate("Dialog", "modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("Dialog", "suprimer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Dialog", "code", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Dialog", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Dialog", "localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Dialog", "consommation", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Dialog", "date_mesure", nullptr));
        radio_eau->setText(QCoreApplication::translate("Dialog", "eau", nullptr));
        radio_elec->setText(QCoreApplication::translate("Dialog", "\303\251lectricit\303\251", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Dialog", "PDF", nullptr));
        btn_afficher_id->setText(QCoreApplication::translate("Dialog", "afficher by id", nullptr));
        btn_recherche->setText(QCoreApplication::translate("Dialog", "recherche", nullptr));
        btn_afficher_prix->setText(QCoreApplication::translate("Dialog", "afficherprix", nullptr));
        btn_afficher->setText(QCoreApplication::translate("Dialog", "afficher", nullptr));
        btn_pdf_avance->setText(QCoreApplication::translate("Dialog", "pdf avance", nullptr));
        btn_qrcode->setText(QCoreApplication::translate("Dialog", "qr_code", nullptr));
        statsButton->setText(QCoreApplication::translate("Dialog", "statsi", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("Dialog", "afichier", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("Dialog", " statistique", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESOURCEWINDOW_H
