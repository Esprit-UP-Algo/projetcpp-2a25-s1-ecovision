/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
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
    QWidget *tab_4;
    QTableWidget *tableWidget_2;
    QWidget *tab_2;
    QFrame *rightContent;
    QLabel *titleLabel;
    QTabWidget *CRUD_Stat;
    QWidget *tabCRUD;
    QPushButton *btnAjouter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QLabel *nom;
    QLineEdit *telephone_2;
    QLabel *adresse;
    QLabel *cin;
    QLabel *telephone;
    QLabel *prenom;
    QLabel *poste;
    QLineEdit *cin_2;
    QLineEdit *adresse_2;
    QLineEdit *prenom_2;
    QLineEdit *nom_2;
    QLineEdit *metier_2;
    QPushButton *btntrier;
    QTableWidget *tableWidget_3;
    QLineEdit *cin_4;
    QLabel *cin_3;
    QLabel *presence;
    QLineEdit *presence_2;
    QPushButton *btnpdf;
    QLabel *salaire;
    QLineEdit *salaire_2;
    QWidget *tabStat;
    QChartView *stat;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1183, 833);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 1461, 861));
        tab = new QWidget();
        tab->setObjectName("tab");
        tabWidget_2 = new QTabWidget(tab);
        tabWidget_2->setObjectName("tabWidget_2");
        tabWidget_2->setGeometry(QRect(10, 20, 1071, 721));
        tabWidget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        btn_ajouter = new QPushButton(tab_3);
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
        label = new QLabel(tab_3);
        label->setObjectName("label");
        label->setGeometry(QRect(110, 70, 111, 31));
        QFont font;
        font.setPointSize(13);
        font.setBold(true);
        label->setFont(font);
        label_3 = new QLabel(tab_3);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(110, 120, 211, 31));
        label_3->setFont(font);
        label_2 = new QLabel(tab_3);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(110, 170, 121, 31));
        label_2->setFont(font);
        label_4 = new QLabel(tab_3);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(110, 220, 181, 41));
        label_4->setFont(font);
        label_5 = new QLabel(tab_3);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(110, 270, 161, 41));
        label_5->setFont(font);
        btn_modifier = new QPushButton(tab_3);
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
        btn_supprimer = new QPushButton(tab_3);
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
        lineEdit_id = new QLineEdit(tab_3);
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
        lineEdit_localisation = new QLineEdit(tab_3);
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
        lineEdit_consommation = new QLineEdit(tab_3);
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
        tableWidget = new QTableWidget(tab_3);
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
        radio_eau = new QRadioButton(tab_3);
        radio_eau->setObjectName("radio_eau");
        radio_eau->setGeometry(QRect(369, 123, 131, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(true);
        radio_eau->setFont(font1);
        radio_elec = new QRadioButton(tab_3);
        radio_elec->setObjectName("radio_elec");
        radio_elec->setGeometry(QRect(529, 123, 131, 31));
        radio_elec->setFont(font1);
        dateEdit_mesure = new QDateEdit(tab_3);
        dateEdit_mesure->setObjectName("dateEdit_mesure");
        dateEdit_mesure->setGeometry(QRect(330, 270, 391, 31));
        QFont font2;
        font2.setPointSize(12);
        dateEdit_mesure->setFont(font2);
        dateEdit_mesure->setDate(QDate(2025, 10, 22));
        pushButton_4 = new QPushButton(tab_3);
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
        btn_afficher_id = new QPushButton(tab_3);
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
        btn_recherche = new QPushButton(tab_3);
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
        btn_afficher_prix = new QPushButton(tab_3);
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
        listWidget = new QListWidget(tab_3);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(710, 450, 256, 192));
        btn_afficher = new QPushButton(tab_3);
        btn_afficher->setObjectName("btn_afficher");
        btn_afficher->setGeometry(QRect(970, 670, 20, 20));
        btn_pdf_avance = new QPushButton(tab_3);
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
        btn_qrcode = new QPushButton(tab_3);
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
        statsButton = new QPushButton(tab_3);
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
        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName("tab_4");
        tableWidget_2 = new QTableWidget(tab_4);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(110, 100, 731, 411));
        tabWidget_2->addTab(tab_4, QString());
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        rightContent = new QFrame(tab_2);
        rightContent->setObjectName("rightContent");
        rightContent->setGeometry(QRect(0, 10, 1191, 761));
        rightContent->setStyleSheet(QString::fromUtf8("\n"
"QFrame {\n"
"    background-color: white;\n"
"    border-radius: 10px;\n"
"    margin: 5px;\n"
"}\n"
"QLabel {\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"    color: #333;\n"
"}\n"
"     "));
        titleLabel = new QLabel(rightContent);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(QRect(14, 14, 233, 62));
        titleLabel->setStyleSheet(QString::fromUtf8("\n"
"font-size: 24px;\n"
"font-weight: bold;\n"
"color: #2f85c9;\n"
"padding: 10px;\n"
"      "));
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);
        CRUD_Stat = new QTabWidget(rightContent);
        CRUD_Stat->setObjectName("CRUD_Stat");
        CRUD_Stat->setGeometry(QRect(40, 70, 1061, 681));
        tabCRUD = new QWidget();
        tabCRUD->setObjectName("tabCRUD");
        btnAjouter = new QPushButton(tabCRUD);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setGeometry(QRect(550, 360, 491, 36));
        btnAjouter->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
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
        btnModifier = new QPushButton(tabCRUD);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setGeometry(QRect(550, 400, 491, 36));
        btnModifier->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
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
        btnSupprimer = new QPushButton(tabCRUD);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setGeometry(QRect(550, 520, 491, 36));
        btnSupprimer->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
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
        nom = new QLabel(tabCRUD);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(550, 30, 121, 31));
        telephone_2 = new QLineEdit(tabCRUD);
        telephone_2->setObjectName("telephone_2");
        telephone_2->setGeometry(QRect(680, 190, 331, 26));
        adresse = new QLabel(tabCRUD);
        adresse->setObjectName("adresse");
        adresse->setGeometry(QRect(550, 150, 121, 31));
        cin = new QLabel(tabCRUD);
        cin->setObjectName("cin");
        cin->setGeometry(QRect(550, 110, 121, 31));
        telephone = new QLabel(tabCRUD);
        telephone->setObjectName("telephone");
        telephone->setGeometry(QRect(550, 190, 121, 31));
        prenom = new QLabel(tabCRUD);
        prenom->setObjectName("prenom");
        prenom->setGeometry(QRect(550, 70, 121, 31));
        poste = new QLabel(tabCRUD);
        poste->setObjectName("poste");
        poste->setGeometry(QRect(550, 230, 121, 31));
        cin_2 = new QLineEdit(tabCRUD);
        cin_2->setObjectName("cin_2");
        cin_2->setGeometry(QRect(680, 110, 331, 26));
        adresse_2 = new QLineEdit(tabCRUD);
        adresse_2->setObjectName("adresse_2");
        adresse_2->setGeometry(QRect(680, 150, 331, 26));
        prenom_2 = new QLineEdit(tabCRUD);
        prenom_2->setObjectName("prenom_2");
        prenom_2->setGeometry(QRect(680, 70, 331, 26));
        nom_2 = new QLineEdit(tabCRUD);
        nom_2->setObjectName("nom_2");
        nom_2->setGeometry(QRect(680, 30, 331, 26));
        metier_2 = new QLineEdit(tabCRUD);
        metier_2->setObjectName("metier_2");
        metier_2->setGeometry(QRect(680, 230, 331, 26));
        btntrier = new QPushButton(tabCRUD);
        btntrier->setObjectName("btntrier");
        btntrier->setGeometry(QRect(550, 480, 491, 36));
        btntrier->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
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
        tableWidget_3 = new QTableWidget(tabCRUD);
        if (tableWidget_3->columnCount() < 8)
            tableWidget_3->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(4, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(5, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(6, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget_3->setHorizontalHeaderItem(7, __qtablewidgetitem12);
        tableWidget_3->setObjectName("tableWidget_3");
        tableWidget_3->setGeometry(QRect(-10, 90, 561, 441));
        cin_4 = new QLineEdit(tabCRUD);
        cin_4->setObjectName("cin_4");
        cin_4->setGeometry(QRect(230, 50, 291, 26));
        cin_4->setStyleSheet(QString::fromUtf8("background-color:  #3C9ED9;"));
        cin_3 = new QLabel(tabCRUD);
        cin_3->setObjectName("cin_3");
        cin_3->setGeometry(QRect(0, 50, 231, 31));
        presence = new QLabel(tabCRUD);
        presence->setObjectName("presence");
        presence->setGeometry(QRect(550, 270, 121, 31));
        presence_2 = new QLineEdit(tabCRUD);
        presence_2->setObjectName("presence_2");
        presence_2->setGeometry(QRect(680, 270, 331, 28));
        btnpdf = new QPushButton(tabCRUD);
        btnpdf->setObjectName("btnpdf");
        btnpdf->setGeometry(QRect(550, 440, 491, 36));
        btnpdf->setStyleSheet(QString::fromUtf8("/* \303\211tat normal (non s\303\251lectionn\303\251) : blanc */\n"
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
        salaire = new QLabel(tabCRUD);
        salaire->setObjectName("salaire");
        salaire->setGeometry(QRect(550, 309, 121, 31));
        salaire_2 = new QLineEdit(tabCRUD);
        salaire_2->setObjectName("salaire_2");
        salaire_2->setGeometry(QRect(680, 310, 171, 24));
        CRUD_Stat->addTab(tabCRUD, QString());
        tabStat = new QWidget();
        tabStat->setObjectName("tabStat");
        stat = new QChartView(tabStat);
        stat->setObjectName("stat");
        stat->setGeometry(QRect(0, 0, 1041, 431));
        CRUD_Stat->addTab(tabStat, QString());
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1183, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);
        CRUD_Stat->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_ajouter->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "code:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "type :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "localisation", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "consommation :", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "date_mesure :", nullptr));
        btn_modifier->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("MainWindow", "suprimer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "code", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "localisation", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "consommation", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "date_mesure", nullptr));
        radio_eau->setText(QCoreApplication::translate("MainWindow", "eau", nullptr));
        radio_elec->setText(QCoreApplication::translate("MainWindow", "\303\251lectricit\303\251", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        btn_afficher_id->setText(QCoreApplication::translate("MainWindow", "afficher by id", nullptr));
        btn_recherche->setText(QCoreApplication::translate("MainWindow", "recherche", nullptr));
        btn_afficher_prix->setText(QCoreApplication::translate("MainWindow", "afficherprix", nullptr));
        btn_afficher->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        btn_pdf_avance->setText(QCoreApplication::translate("MainWindow", "pdf avance", nullptr));
        btn_qrcode->setText(QCoreApplication::translate("MainWindow", "qr_code", nullptr));
        statsButton->setText(QCoreApplication::translate("MainWindow", "statsi", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QCoreApplication::translate("MainWindow", "afichier", nullptr));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QCoreApplication::translate("MainWindow", " statistique", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "resource", nullptr));
        titleLabel->setText(QCoreApplication::translate("MainWindow", "Gestion Employ\303\251e", nullptr));
        btnAjouter->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        nom->setText(QCoreApplication::translate("MainWindow", "nom:", nullptr));
        adresse->setText(QCoreApplication::translate("MainWindow", "adresse:", nullptr));
        cin->setText(QCoreApplication::translate("MainWindow", "cin:", nullptr));
        telephone->setText(QCoreApplication::translate("MainWindow", "telephone:", nullptr));
        prenom->setText(QCoreApplication::translate("MainWindow", "prenom:", nullptr));
        poste->setText(QCoreApplication::translate("MainWindow", "poste", nullptr));
        btntrier->setText(QCoreApplication::translate("MainWindow", "trier", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_3->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_3->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "NOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget_3->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "PRENOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget_3->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "TELEPHONE", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget_3->horizontalHeaderItem(4);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "ADRESSE", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget_3->horizontalHeaderItem(5);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "POSTE", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget_3->horizontalHeaderItem(6);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "PRESENCE", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget_3->horizontalHeaderItem(7);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "SALAIRE", nullptr));
        cin_4->setPlaceholderText(QString());
        cin_3->setText(QCoreApplication::translate("MainWindow", "Saisir le cin \303\240 chercher : ", nullptr));
        presence->setText(QCoreApplication::translate("MainWindow", "presence:", nullptr));
        btnpdf->setText(QCoreApplication::translate("MainWindow", "pdf", nullptr));
        salaire->setText(QCoreApplication::translate("MainWindow", "salaire:", nullptr));
        CRUD_Stat->setTabText(CRUD_Stat->indexOf(tabCRUD), QCoreApplication::translate("MainWindow", "Inforamtions", nullptr));
        CRUD_Stat->setTabText(CRUD_Stat->indexOf(tabStat), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "employe", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
