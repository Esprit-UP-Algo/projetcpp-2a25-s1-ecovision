/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
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
    QLineEdit *lineEdit_7;
    QLabel *label_8;
    QComboBox *comboBox;
    QTableWidget *tableWidget;
    QRadioButton *radio_eau;
    QRadioButton *radio_elec;
    QDateEdit *dateEdit_mesure;
    QTextEdit *textEdit;
    QPushButton *pushButton_4;
    QLabel *label_6;
    QPushButton *btn_afficher;
    QPushButton *btn_recherche;
    QWidget *tab_2;
    QTableWidget *tableWidget_2;
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
        pushButton_5_ressources->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background et couleurs invers\303\251es */\n"
"#pushButton_5_ressources {\n"
"    width: 100px;\n"
"    height: 40px;\n"
"    border-radius: 20px;                  /* demi-hauteur \342\206\222 style pill */\n"
"    border: 1px solid rgba(47, 133, 201, 90); /* inverse de l\342\200\231original */\n"
"    background-color: #ffffff;            /* couleur invers\303\251e du fond */\n"
"    background-image: url(:/images/icone.png); /* image */\n"
"    background-repeat: no-repeat;\n"
"    background-position: center;\n"
"    background-size: 20px 20px;           /* image plus petite que le bouton */\n"
"    color: #5cb4f2;                        /* couleur texte invers\303\251e */\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    text-align: center;\n"
"    transition: all 0.3s ease;\n"
"    padding-left: 10px;                    /* espace texte */\n"
"    padding-right: 10px;\n"
"}\n"
"\n"
"/* Hover invers\303\251 */\n"
"#pushButton_5_ressources:hover {\n"
"    backgrou"
                        "nd: #5cb4f2;                   /* couleur originale du fond */\n"
"    color: white;                           /* texte blanc sur hover */\n"
"}\n"
"\n"
"/* Press\303\251 invers\303\251 */\n"
"#pushButton_5_ressources:pressed {\n"
"    background-color: #ffffff;              /* couleur du fond invers\303\251e au clic */\n"
"    color: #2f85c9;                         /* texte bleu fonc\303\251 au clic */\n"
"    transform: scale(0.95);\n"
"}"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(40, 40, 241, 141));
        label_9->setPixmap(QPixmap(QString::fromUtf8("image.png")));
        label_9->setScaledContents(true);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(330, 0, 931, 691));
        tabWidget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tab = new QWidget();
        tab->setObjectName("tab");
        btn_ajouter = new QPushButton(tab);
        btn_ajouter->setObjectName("btn_ajouter");
        btn_ajouter->setGeometry(QRect(130, 320, 151, 41));
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
        btn_modifier->setGeometry(QRect(320, 320, 151, 41));
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
        btn_supprimer->setGeometry(QRect(510, 320, 151, 41));
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
        lineEdit_7 = new QLineEdit(tab);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(200, 400, 201, 31));
        lineEdit_7->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_8 = new QLabel(tab);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(530, 390, 111, 41));
        label_8->setFont(font);
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(650, 400, 101, 31));
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
        textEdit = new QTextEdit(tab);
        textEdit->setObjectName("textEdit");
        textEdit->setGeometry(QRect(710, 450, 191, 191));
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
        label_6 = new QLabel(tab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(60, 400, 131, 31));
        label_6->setFont(font);
        btn_afficher = new QPushButton(tab);
        btn_afficher->setObjectName("btn_afficher");
        btn_afficher->setGeometry(QRect(710, 320, 151, 41));
        btn_afficher->setStyleSheet(QString::fromUtf8("/* Bouton arrondi avec image comme background */\n"
"#btn_afficher{\n"
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
"#btn_afficher:hover {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,\n"
"                                stop:0 #5cb4f2, stop:1 #ffffff);\n"
"    color: #2f85c9;\n"
"}\n"
"\n"
"/* Press\303\251 */\n"
""
                        "#btn_afficher:pressed {\n"
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
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tableWidget_2 = new QTableWidget(tab_2);
        if (tableWidget_2->columnCount() < 1)
            tableWidget_2->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(10, 20, 901, 611));
        tableWidget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
""));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1274, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


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
        btn_ajouter->setText(QCoreApplication::translate("MainWindow", "ajouter", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "code:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "type :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "localisation", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "consommation :", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "date_mesure :", nullptr));
        btn_modifier->setText(QCoreApplication::translate("MainWindow", "modifier", nullptr));
        btn_supprimer->setText(QCoreApplication::translate("MainWindow", "suprimer", nullptr));
        lineEdit_7->setText(QString());
        lineEdit_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "trier date:", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "date", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "0-1000dt", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("MainWindow", "1000dt-5000dt", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("MainWindow", "5000dt-10000dt", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("MainWindow", "10000dt-20000dt", nullptr));

        comboBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "date", nullptr));
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
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">prix</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "PDF", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Recherche:", nullptr));
        btn_afficher->setText(QCoreApplication::translate("MainWindow", "afficher", nullptr));
        btn_recherche->setText(QCoreApplication::translate("MainWindow", "recherche", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "afichier", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "tableau statistique", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", " statistique", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
