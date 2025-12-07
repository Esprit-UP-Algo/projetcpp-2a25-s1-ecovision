/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *id;
    QLineEdit *nom;
    QLineEdit *emplacement;
    QLineEdit *capacite;
    QLineEdit *tariff;
    QPushButton *ajouterclient;
    QPushButton *modifierclient;
    QPushButton *qrcodegen;
    QLabel *qrcodecommande;
    QLabel *label_23;
    QLineEdit *statut;
    QLabel *label_6;
    QLineEdit *placesDisponibles;
    QLabel *label_7;
    QWidget *tab_2;
    QTableView *tableView;
    QLineEdit *rechclient;
    QPushButton *pushButton;
    QPushButton *supprclient;
    QPushButton *tricin;
    QPushButton *trnom;
    QPushButton *pushButton_2;
    QLineEdit *suppc;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1092, 672);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, -10, 1091, 591));
        tabWidget->setStyleSheet(QString::fromUtf8("background-image: url(:/new/prefix1/backg2.jpg);"));
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(70, 100, 55, 16));
        label_2 = new QLabel(tab);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 140, 55, 16));
        label_3 = new QLabel(tab);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(70, 180, 111, 20));
        label_4 = new QLabel(tab);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(70, 220, 71, 16));
        label_5 = new QLabel(tab);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(70, 300, 111, 16));
        id = new QLineEdit(tab);
        id->setObjectName("id");
        id->setGeometry(QRect(200, 100, 113, 22));
        nom = new QLineEdit(tab);
        nom->setObjectName("nom");
        nom->setGeometry(QRect(200, 140, 113, 22));
        emplacement = new QLineEdit(tab);
        emplacement->setObjectName("emplacement");
        emplacement->setGeometry(QRect(200, 180, 113, 22));
        capacite = new QLineEdit(tab);
        capacite->setObjectName("capacite");
        capacite->setGeometry(QRect(200, 220, 113, 22));
        tariff = new QLineEdit(tab);
        tariff->setObjectName("tariff");
        tariff->setGeometry(QRect(200, 300, 113, 22));
        ajouterclient = new QPushButton(tab);
        ajouterclient->setObjectName("ajouterclient");
        ajouterclient->setGeometry(QRect(70, 400, 93, 28));
        modifierclient = new QPushButton(tab);
        modifierclient->setObjectName("modifierclient");
        modifierclient->setGeometry(QRect(220, 400, 93, 28));
        qrcodegen = new QPushButton(tab);
        qrcodegen->setObjectName("qrcodegen");
        qrcodegen->setGeometry(QRect(620, 360, 211, 61));
        qrcodegen->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    /* Style de base */\n"
"    background-color: #3498db;  /* Couleur de fond bleu */\n"
"    color: #ffffff;  /* Couleur du texte blanc */\n"
"    border-radius: 15px;  /* Bords arrondis avec un rayon de 15px */\n"
"    padding: 10px 20px;  /* Marges int\303\251rieures */\n"
"    font-family: 'Arial', sans-serif;  /* Police Arial */\n"
"    font-size: 16px;  /* Taille de la police */\n"
"    border: 2px solid #2980b9;  /* Bordure avec une couleur l\303\251g\303\250rement plus fonc\303\251e */\n"
"    transition: all 0.3s ease;  /* Effet de transition lisse */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* Effet lors du survol */\n"
"    background-color: #2980b9;  /* Couleur de fond l\303\251g\303\250rement plus fonc\303\251e */\n"
"    border: 2px solid #1f618d;  /* Couleur de la bordure encore plus fonc\303\251e */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    /* Effet lors du clic */\n"
"    background-color: #1f618d;  /* Couleur de fond encore plus fonc\303\251e */\n"
"    border: 2px sol"
                        "id #154360;  /* Bordure tr\303\250s fonc\303\251e */\n"
"    color: #ecf0f1;  /* L\303\251g\303\250re variation de la couleur du texte */\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    /* Style du bouton d\303\251sactiv\303\251 */\n"
"    background-color: #bdc3c7;  /* Couleur de fond gris clair */\n"
"    color: #7f8c8d;  /* Couleur du texte gris fonc\303\251 */\n"
"    border: 2px solid #95a5a6;  /* Bordure gris fonc\303\251 */\n"
"}\n"
"\n"
"QPushButton {\n"
"    /* Ombre port\303\251e */\n"
"    box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.3);  /* L\303\251g\303\250re ombre port\303\251e */\n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ImgSrc/qr.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        qrcodegen->setIcon(icon);
        qrcodecommande = new QLabel(tab);
        qrcodecommande->setObjectName("qrcodecommande");
        qrcodecommande->setGeometry(QRect(620, 120, 200, 200));
        label_23 = new QLabel(tab);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(680, 60, 101, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Rockwell Condensed")});
        font.setPointSize(12);
        label_23->setFont(font);
        label_23->setFrameShape(QFrame::Shape::WinPanel);
        statut = new QLineEdit(tab);
        statut->setObjectName("statut");
        statut->setGeometry(QRect(200, 350, 113, 22));
        label_6 = new QLabel(tab);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(70, 350, 111, 16));
        placesDisponibles = new QLineEdit(tab);
        placesDisponibles->setObjectName("placesDisponibles");
        placesDisponibles->setGeometry(QRect(200, 260, 113, 22));
        label_7 = new QLabel(tab);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(70, 260, 111, 16));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tableView = new QTableView(tab_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(40, 70, 1011, 421));
        rechclient = new QLineEdit(tab_2);
        rechclient->setObjectName("rechclient");
        rechclient->setGeometry(QRect(80, 20, 141, 31));
        pushButton = new QPushButton(tab_2);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(230, 20, 121, 41));
        supprclient = new QPushButton(tab_2);
        supprclient->setObjectName("supprclient");
        supprclient->setGeometry(QRect(470, 20, 121, 41));
        tricin = new QPushButton(tab_2);
        tricin->setObjectName("tricin");
        tricin->setGeometry(QRect(690, 20, 111, 41));
        trnom = new QPushButton(tab_2);
        trnom->setObjectName("trnom");
        trnom->setGeometry(QRect(832, 20, 131, 41));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(440, 500, 171, 41));
        suppc = new QLineEdit(tab_2);
        suppc->setObjectName("suppc");
        suppc->setGeometry(QRect(390, 20, 61, 31));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1092, 25));
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
        label->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "NOM :", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "EMPLACEMENT", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "CAPACITE", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "TARIFF", nullptr));
        ajouterclient->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        modifierclient->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        qrcodegen->setText(QCoreApplication::translate("MainWindow", "Generate QR Code", nullptr));
        qrcodecommande->setText(QString());
        label_23->setText(QCoreApplication::translate("MainWindow", "QR CODE", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "STATUT", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "PLACES DISPO.", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Parking", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Chercher", nullptr));
        supprclient->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        tricin->setText(QCoreApplication::translate("MainWindow", "Tri CIN", nullptr));
        trnom->setText(QCoreApplication::translate("MainWindow", "Tri Nom", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Export PDF", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Afficher Parking", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
