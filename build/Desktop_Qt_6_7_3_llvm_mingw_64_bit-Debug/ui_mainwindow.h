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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton_6;
    QLabel *label_19;
    QLabel *label_20;
    QPushButton *pushButton_9;
    QLabel *label_17;
    QTabWidget *tabWidget;
    QWidget *afficher;
    QGroupBox *groupBox_2;
    QTableView *tableView;
    QLineEdit *lineedit_id_rech;
    QLabel *label_49;
    QLabel *label_48;
    QPushButton *supprimerButton;
    QPushButton *pb_pdf;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_23;
    QPushButton *rechercherButton;
    QLineEdit *l5;
    QPushButton *trierButton;
    QPushButton *statistiqueButton;
    QListWidget *listWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *statistiqueButton_2;
    QPushButton *pb_historiqueService;
    QWidget *Ajouter;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *l2;
    QLineEdit *l4;
    QPushButton *ajouterButton;
    QLineEdit *l1;
    QLineEdit *l3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_11;
    QLineEdit *l4_2;
    QLineEdit *l4_3;
    QLabel *label_12;
    QLineEdit *l4_4;
    QLabel *label_13;
    QLineEdit *l4_5;
    QWidget *Modifier;
    QGroupBox *groupBox_3;
    QLabel *label_9;
    QLabel *label_10;
    QLineEdit *l8;
    QPushButton *modifierButton;
    QLineEdit *lineedit_id_modif;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *l7;
    QLabel *label_46;
    QLabel *label_47;
    QPushButton *sendSMSButton;
    QPlainTextEdit *contenu;
    QLabel *label_14;
    QPushButton *pushButton_4;
    QLabel *label_18;
    QPushButton *pushButton_menu;
    QPushButton *pushButton_11;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1444, 1012);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        pushButton_6 = new QPushButton(centralwidget);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(-40, 250, 261, 51));
        pushButton_6->setStyleSheet(QString::fromUtf8("/* Bouton non s\303\251lectionn\303\251 */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look attrayant */\n"
"    color: #FFFFFF;             /* Texte en blanc */\n"
"    border: 3px solid #2A2DAC;  /* Bordure bleu fonc\303\251 pour un bon contraste */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 15pt \"Arial\";      /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Bouton s\303\251lectionn\303\251 */\n"
"QPushButton:checked {\n"
"    background-color: #2A2DAC;  /* Bleu plus fonc\303\251 pour bouton s\303\251lectionn\303\251 */\n"
"    color: #FFFFFF;              /* Texte en blanc */\n"
"    border: 3px solid #353FF2;   /* Bordure bleu vif */\n"
"    border-radius: 12px;         /* Coins arrondis */\n"
"    padding: 8px 12px;           /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 18"
                        "pt \"Arial\";       /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #4A5BCC;   /* Bleu clair au survol */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0; /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070; /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0; /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
""));
        label_19 = new QLabel(centralwidget);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(50, 700, 81, 51));
        label_19->setStyleSheet(QString::fromUtf8("image: url(:/res/n-removebg-preview.png)"));
        label_20 = new QLabel(centralwidget);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(50, 750, 81, 51));
        label_20->setStyleSheet(QString::fromUtf8("image: url(:/res/www-removebg-preview.png)"));
        pushButton_9 = new QPushButton(centralwidget);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(-40, 460, 271, 51));
        pushButton_9->setStyleSheet(QString::fromUtf8("/* Bouton non s\303\251lectionn\303\251 */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look attrayant */\n"
"    color: #FFFFFF;             /* Texte en blanc */\n"
"    border: 3px solid #2A2DAC;  /* Bordure bleu fonc\303\251 pour un bon contraste */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 15pt \"Arial\";      /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Bouton s\303\251lectionn\303\251 */\n"
"QPushButton:checked {\n"
"    background-color: #2A2DAC;  /* Bleu plus fonc\303\251 pour bouton s\303\251lectionn\303\251 */\n"
"    color: #FFFFFF;              /* Texte en blanc */\n"
"    border: 3px solid #353FF2;   /* Bordure bleu vif */\n"
"    border-radius: 12px;         /* Coins arrondis */\n"
"    padding: 8px 12px;           /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 18"
                        "pt \"Arial\";       /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #4A5BCC;   /* Bleu clair au survol */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0; /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070; /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0; /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
""));
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(40, 610, 101, 41));
        label_17->setStyleSheet(QString::fromUtf8("image: url(:/res/vv-removebg-preview.png)\n"
""));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(230, 0, 1675, 861));
        tabWidget->setMinimumSize(QSize(1675, 0));
        tabWidget->setMaximumSize(QSize(1674, 935));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial Rounded MT Bold")});
        font.setPointSize(20);
        tabWidget->setFont(font);
        tabWidget->setStyleSheet(QString::fromUtf8("/* Fond du QTabWidget */\n"
"QTabWidget {\n"
"    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(30, 144, 255, 255), stop:1 rgba(0, 70, 128, 255));\n"
"}\n"
"\n"
"/* Style des onglets */\n"
"QTabBar::tab {\n"
"    background-color: #5cb5f2; /* Remplac\303\251 par la couleur #3049D9 */\n"
"    color: #FFF9C4; /* Texte blanc */\n"
"   border: 3px solid #2A2DAC;\n"
"   border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 12px 20px; /* Espacement interne - ajustez selon vos besoins */\n"
"    text-align: center; /* Centrer le texte */\n"
"}\n"
"\n"
"/* Style des onglets lorsqu'ils sont survol\303\251s */\n"
"QTabBar::tab:hover {\n"
"    background-color: #435AD9; /* Changer la couleur au survol */\n"
"}\n"
"\n"
"/* Style des onglets actifs */\n"
"QTabBar::tab:pressed {\n"
"    background-color: #353FF2; /* Conserver la m\303\252me couleur que pour l'onglet actif */\n"
"}\n"
"\n"
"\n"
"/* Style du contenu des onglets */\n"
"QTabWidget::pane {\n"
"    border: 2px solid #0000"
                        "00; /* Bordure bleu fonc\303\251 pour le contenu des onglets */\n"
"    border-top: 0; /* Supprimer la bordure sup\303\251rieure */\n"
"}\n"
"\n"
"/* Style des boutons de fermeture des onglets */\n"
"QTabBar::close-button {\n"
"    image: url(close.png); /* Remplacez par le chemin de votre ic\303\264ne de fermeture */\n"
"    subcontrol-position: right; /* Position \303\240 droite du texte de l'onglet */\n"
"}\n"
"\n"
"QTabBar::close-button:hover {\n"
"    image: url(close-hover.png); /* Ic\303\264ne de fermeture au survol */\n"
"}\n"
"\n"
"/* Ajustement de la police */\n"
"QTabBar {\n"
"    font-size: 16px; /* Taille de la police pour les onglets */\n"
"    qproperty-drawBase: 0; /* D\303\251sactiver le dessin de la base (fond) de l'onglet */\n"
"}\n"
"\n"
""));
        tabWidget->setIconSize(QSize(16, 16));
        afficher = new QWidget();
        afficher->setObjectName("afficher");
        groupBox_2 = new QGroupBox(afficher);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 10, 861, 611));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Trebuchet MS")});
        font1.setPointSize(24);
        font1.setBold(true);
        groupBox_2->setFont(font1);
        groupBox_2->setStyleSheet(QString::fromUtf8("/* Style de base pour la QGroupBox */\n"
"QGroupBox {\n"
"    border: 4px solid #999DF2; /* Bordure de la m\303\252me couleur que le fond du titre */\n"
"    border-radius: 25px; /* Coins arrondis */\n"
"    margin-top: 10px; /* Marge en haut */\n"
"}\n"
"\n"
"/* Style du titre de la QGroupBox */\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 3px;\n"
"    background-color: #999DF2; /* Fond remplac\303\251 par #999DF2 */\n"
"    color: #ffffff; /* Texte blanc pour le titre */\n"
"    border-radius: 10px; /* Coins arrondis du titre */\n"
"}\n"
"\n"
"/* Style des QLabel \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLabel {\n"
"    color: #000000; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QRadioButton \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QRadioButton {\n"
"    color: #333333; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QComboBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QComboBox {\n"
"    background"
                        "-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QLineEdit \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLineEdit {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QSpinBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QSpinBox {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sacti"
                        "v\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0; /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0; /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}"));
        groupBox_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tableView = new QTableView(groupBox_2);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(25, 51, 811, 311));
        lineedit_id_rech = new QLineEdit(groupBox_2);
        lineedit_id_rech->setObjectName("lineedit_id_rech");
        lineedit_id_rech->setGeometry(QRect(590, 440, 221, 51));
        QFont font2;
        font2.setPointSize(14);
        lineedit_id_rech->setFont(font2);
        lineedit_id_rech->setStyleSheet(QString::fromUtf8("border-radius :15px"));
        lineedit_id_rech->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_49 = new QLabel(groupBox_2);
        label_49->setObjectName("label_49");
        label_49->setGeometry(QRect(420, 370, 391, 31));
        label_49->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px;\n"
"background-color: rgb(92, 181, 242);"));
        label_49->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_48 = new QLabel(groupBox_2);
        label_48->setObjectName("label_48");
        label_48->setGeometry(QRect(410, 450, 151, 31));
        label_48->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px; \n"
"background-color: #5cb5f2;"));
        label_48->setAlignment(Qt::AlignmentFlag::AlignCenter);
        supprimerButton = new QPushButton(groupBox_2);
        supprimerButton->setObjectName("supprimerButton");
        supprimerButton->setGeometry(QRect(310, 530, 221, 51));
        supprimerButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 10pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        pb_pdf = new QPushButton(groupBox_2);
        pb_pdf->setObjectName("pb_pdf");
        pb_pdf->setGeometry(QRect(80, 530, 221, 51));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Times New Roman")});
        font3.setPointSize(10);
        font3.setWeight(QFont::Normal);
        font3.setItalic(false);
        pb_pdf->setFont(font3);
        pb_pdf->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 10pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        label_21 = new QLabel(afficher);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(890, 30, 351, 31));
        label_21->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_22 = new QLabel(afficher);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(880, 160, 341, 31));
        label_22->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_23 = new QLabel(afficher);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(880, 240, 381, 31));
        label_23->setStyleSheet(QString::fromUtf8("font: italic 7pt \"Times New Roman\";\n"
"border-radius :15px"));
        rechercherButton = new QPushButton(afficher);
        rechercherButton->setObjectName("rechercherButton");
        rechercherButton->setGeometry(QRect(890, 110, 221, 41));
        rechercherButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        l5 = new QLineEdit(afficher);
        l5->setObjectName("l5");
        l5->setGeometry(QRect(930, 70, 141, 31));
        trierButton = new QPushButton(afficher);
        trierButton->setObjectName("trierButton");
        trierButton->setGeometry(QRect(900, 190, 101, 41));
        trierButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        statistiqueButton = new QPushButton(afficher);
        statistiqueButton->setObjectName("statistiqueButton");
        statistiqueButton->setGeometry(QRect(900, 280, 191, 51));
        statistiqueButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        listWidget = new QListWidget(afficher);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(880, 340, 341, 71));
        verticalLayoutWidget = new QWidget(afficher);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(890, 490, 361, 231));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        statistiqueButton_2 = new QPushButton(afficher);
        statistiqueButton_2->setObjectName("statistiqueButton_2");
        statistiqueButton_2->setGeometry(QRect(880, 430, 341, 51));
        statistiqueButton_2->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        pb_historiqueService = new QPushButton(afficher);
        pb_historiqueService->setObjectName("pb_historiqueService");
        pb_historiqueService->setGeometry(QRect(270, 660, 221, 51));
        pb_historiqueService->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        tabWidget->addTab(afficher, QString());
        Ajouter = new QWidget();
        Ajouter->setObjectName("Ajouter");
        groupBox = new QGroupBox(Ajouter);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 10, 1171, 761));
        groupBox->setFont(font1);
        groupBox->setStyleSheet(QString::fromUtf8("/* Style de base pour la QGroupBox */\n"
"QGroupBox {\n"
"    border: 4px solid #999DF2; /* Bordure de la m\303\252me couleur que le fond du titre */\n"
"    border-radius: 25px; /* Coins arrondis */\n"
"    margin-top: 10px; /* Marge en haut */\n"
"}\n"
"\n"
"/* Style du titre de la QGroupBox */\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 3px;\n"
"    background-color: #999DF2; /* Fond remplac\303\251 par #999DF2 */\n"
"    color: #ffffff; /* Texte blanc pour le titre */\n"
"    border-radius: 10px; /* Coins arrondis du titre */\n"
"\n"
"}\n"
"\n"
"/* Style des QLabel \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLabel {\n"
"    color: #000000; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QRadioButton \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QRadioButton {\n"
"    color: #333333; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QComboBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QComboBox {\n"
"    backg"
                        "round-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QLineEdit \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLineEdit {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QSpinBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QSpinBox {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251"
                        "sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0; /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0; /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}"));
        groupBox->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 210, 161, 31));
        label->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(80, 150, 131, 31));
        label_2->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(110, 100, 171, 31));
        label_3->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(120, 280, 131, 31));
        label_4->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l2 = new QLineEdit(groupBox);
        l2->setObjectName("l2");
        l2->setGeometry(QRect(350, 140, 471, 41));
        l2->setFont(font2);
        l2->setStyleSheet(QString::fromUtf8("border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l4 = new QLineEdit(groupBox);
        l4->setObjectName("l4");
        l4->setGeometry(QRect(350, 270, 471, 41));
        l4->setFont(font2);
        l4->setStyleSheet(QString::fromUtf8("border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        ajouterButton = new QPushButton(groupBox);
        ajouterButton->setObjectName("ajouterButton");
        ajouterButton->setGeometry(QRect(460, 660, 221, 51));
        ajouterButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #435AD9;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        l1 = new QLineEdit(groupBox);
        l1->setObjectName("l1");
        l1->setGeometry(QRect(350, 90, 471, 41));
        l1->setFont(font2);
        l1->setStyleSheet(QString::fromUtf8("border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l1->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l1->setReadOnly(false);
        l3 = new QLineEdit(groupBox);
        l3->setObjectName("l3");
        l3->setGeometry(QRect(350, 200, 471, 41));
        l3->setFont(font2);
        l3->setStyleSheet(QString::fromUtf8("border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(830, 280, 131, 31));
        label_5->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_5->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(970, 280, 181, 31));
        label_6->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_6->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(110, 360, 211, 31));
        label_7->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_7->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_11 = new QLabel(groupBox);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(90, 510, 161, 31));
        label_11->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_11->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l4_2 = new QLineEdit(groupBox);
        l4_2->setObjectName("l4_2");
        l4_2->setGeometry(QRect(350, 350, 471, 41));
        l4_2->setFont(font2);
        l4_2->setStyleSheet(QString::fromUtf8("border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l4_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l4_3 = new QLineEdit(groupBox);
        l4_3->setObjectName("l4_3");
        l4_3->setGeometry(QRect(350, 500, 471, 41));
        l4_3->setFont(font2);
        l4_3->setStyleSheet(QString::fromUtf8("\n"
"border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l4_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_12 = new QLabel(groupBox);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(70, 590, 161, 31));
        label_12->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_12->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l4_4 = new QLineEdit(groupBox);
        l4_4->setObjectName("l4_4");
        l4_4->setGeometry(QRect(350, 570, 471, 41));
        l4_4->setFont(font2);
        l4_4->setStyleSheet(QString::fromUtf8("\n"
"border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l4_4->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(110, 440, 211, 31));
        label_13->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_13->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l4_5 = new QLineEdit(groupBox);
        l4_5->setObjectName("l4_5");
        l4_5->setGeometry(QRect(350, 430, 471, 41));
        l4_5->setFont(font2);
        l4_5->setStyleSheet(QString::fromUtf8("\n"
"border-radius :15px;\n"
"color: rgb(92, 181, 242);"));
        l4_5->setAlignment(Qt::AlignmentFlag::AlignCenter);
        tabWidget->addTab(Ajouter, QString());
        Modifier = new QWidget();
        Modifier->setObjectName("Modifier");
        groupBox_3 = new QGroupBox(Modifier);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(10, 10, 1191, 671));
        groupBox_3->setFont(font1);
        groupBox_3->setStyleSheet(QString::fromUtf8("/* Style de base pour la QGroupBox */\n"
"QGroupBox {\n"
"    border: 4px solid #999DF2; /* Bordure de la m\303\252me couleur que le fond du titre */\n"
"    border-radius: 25px; /* Coins arrondis */\n"
"    margin-top: 10px; /* Marge en haut */\n"
"}\n"
"\n"
"/* Style du titre de la QGroupBox */\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top center;\n"
"    padding: 0 3px;\n"
"    background-color: #999DF2; /* Fond remplac\303\251 par #999DF2 */\n"
"    color: #ffffff; /* Texte blanc pour le titre */\n"
"    border-radius: 10px; /* Coins arrondis du titre */\n"
"}\n"
"\n"
"/* Style des QLabel \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLabel {\n"
"    color: #000000; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QRadioButton \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QRadioButton {\n"
"    color: #333333; /* Couleur du texte */\n"
"}\n"
"\n"
"/* Style des QComboBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QComboBox {\n"
"    background"
                        "-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QLineEdit \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QLineEdit {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style des QSpinBox \303\240 l'int\303\251rieur de la QGroupBox */\n"
"QSpinBox {\n"
"    background-color: #ffffff; /* Fond blanc */\n"
"    color: #333333; /* Couleur du texte */\n"
"    border: 1px solid #3498db; /* Bordure bleue */\n"
"    border-radius: 3px; /* Coins arrondis */\n"
"    padding: 2px; /* Espacement interne */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sacti"
                        "v\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0; /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0; /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}"));
        groupBox_3->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(10, 190, 181, 31));
        label_9->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_9->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_10 = new QLabel(groupBox_3);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(30, 280, 141, 31));
        label_10->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_10->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l8 = new QLineEdit(groupBox_3);
        l8->setObjectName("l8");
        l8->setGeometry(QRect(230, 260, 471, 51));
        l8->setFont(font2);
        l8->setStyleSheet(QString::fromUtf8("border-radius :15px"));
        l8->setAlignment(Qt::AlignmentFlag::AlignCenter);
        modifierButton = new QPushButton(groupBox_3);
        modifierButton->setObjectName("modifierButton");
        modifierButton->setGeometry(QRect(370, 430, 221, 51));
        modifierButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #435AD9;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        lineedit_id_modif = new QLineEdit(groupBox_3);
        lineedit_id_modif->setObjectName("lineedit_id_modif");
        lineedit_id_modif->setGeometry(QRect(940, 250, 221, 51));
        lineedit_id_modif->setFont(font2);
        lineedit_id_modif->setStyleSheet(QString::fromUtf8("border-radius :15px"));
        lineedit_id_modif->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(770, 260, 141, 31));
        label_15->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_15->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(760, 130, 371, 31));
        label_16->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_16->setAlignment(Qt::AlignmentFlag::AlignCenter);
        l7 = new QLineEdit(groupBox_3);
        l7->setObjectName("l7");
        l7->setGeometry(QRect(230, 170, 471, 51));
        l7->setFont(font2);
        l7->setStyleSheet(QString::fromUtf8("border-radius :15px"));
        l7->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_46 = new QLabel(groupBox_3);
        label_46->setObjectName("label_46");
        label_46->setGeometry(QRect(210, 350, 201, 31));
        label_46->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_46->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_47 = new QLabel(groupBox_3);
        label_47->setObjectName("label_47");
        label_47->setGeometry(QRect(500, 350, 201, 31));
        label_47->setStyleSheet(QString::fromUtf8("font: italic 14pt \"Times New Roman\";\n"
"border-radius :15px"));
        label_47->setAlignment(Qt::AlignmentFlag::AlignCenter);
        sendSMSButton = new QPushButton(groupBox_3);
        sendSMSButton->setObjectName("sendSMSButton");
        sendSMSButton->setGeometry(QRect(940, 520, 221, 51));
        sendSMSButton->setStyleSheet(QString::fromUtf8("/* Style de base pour le QPushButton */\n"
"QPushButton {\n"
"    background-color: #435AD9;  /* Bleu vif pour un look frais et moderne */\n"
"    color: #ffffff;              /* Texte blanc */\n"
"    border: 3px solid #435AD9;  /* Bordure de la m\303\252me couleur que le fond */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne */\n"
"    font: 75 18pt \"Times New Roman\"; /* Police en gras */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #5f7cd8;   /* Bleu plus clair au survol */\n"
"    border: 3px solid #5f7cd8;   /* Bordure de la m\303\252me couleur que le fond survol\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est press\303\251 (clic) */\n"
"QPushButton:pressed {\n"
"    background-color: #3b4fbf;   /* Bleu fonc\303\251 au clic */\n"
"    border: 3px solid #3b4fbf;   /* Bordure de la m\303\252me couleur que le fond press\303\251 */\n"
"}\n"
"\n"
"/* "
                        "Style du QPushButton lorsqu'il est en focus (tabulation) */\n"
"QPushButton:focus {\n"
"    outline: none;               /* Supprimer le contour par d\303\251faut */\n"
"    border: 2px solid #2e8a56;  /* Bordure plus fonc\303\251e lorsqu'en focus */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0;   /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070;              /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0;   /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
"\n"
"/* Style g\303\251n\303\251ral pour les widgets lorsqu'ils sont d\303\251sactiv\303\251s */\n"
"QWidget:disabled {\n"
"    background-color: #f0f0f0;   /* Fond gris clair pour les widgets d\303\251sactiv\303\251s */\n"
"    color: #a0a0a0;              /* Couleur du texte grise pour les widgets d\303\251sactiv\303\251s */\n"
"}\n"
""));
        contenu = new QPlainTextEdit(groupBox_3);
        contenu->setObjectName("contenu");
        contenu->setGeometry(QRect(940, 350, 211, 151));
        tabWidget->addTab(Modifier, QString());
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(-30, 30, 241, 211));
        label_14->setStyleSheet(QString::fromUtf8("image: url(:/res/y.png)"));
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(-40, 390, 261, 51));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Arial")});
        font4.setPointSize(15);
        font4.setWeight(QFont::Normal);
        font4.setItalic(false);
        pushButton_4->setFont(font4);
        pushButton_4->setStyleSheet(QString::fromUtf8("/* Bouton non s\303\251lectionn\303\251 */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look attrayant */\n"
"    color: #FFFFFF;             /* Texte en blanc */\n"
"    border: 3px solid #2A2DAC;  /* Bordure bleu fonc\303\251 pour un bon contraste */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 15pt \"Arial\";      /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Bouton s\303\251lectionn\303\251 */\n"
"QPushButton:checked {\n"
"    background-color: #2A2DAC;  /* Bleu plus fonc\303\251 pour bouton s\303\251lectionn\303\251 */\n"
"    color: #FFFFFF;              /* Texte en blanc */\n"
"    border: 3px solid #353FF2;   /* Bordure bleu vif */\n"
"    border-radius: 12px;         /* Coins arrondis */\n"
"    padding: 8px 12px;           /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 18"
                        "pt \"Arial\";       /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #4A5BCC;   /* Bleu clair au survol */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0; /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070; /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0; /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
""));
        label_18 = new QLabel(centralwidget);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(50, 660, 81, 41));
        label_18->setStyleSheet(QString::fromUtf8("image: url(:/res/pp-removebg-preview.png)"));
        pushButton_menu = new QPushButton(centralwidget);
        pushButton_menu->setObjectName("pushButton_menu");
        pushButton_menu->setGeometry(QRect(-40, 320, 261, 51));
        pushButton_menu->setStyleSheet(QString::fromUtf8("/* Bouton non s\303\251lectionn\303\251 */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look attrayant */\n"
"    color: #FFFFFF;             /* Texte en blanc */\n"
"    border: 3px solid #2A2DAC;  /* Bordure bleu fonc\303\251 pour un bon contraste */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 15pt \"Arial\";      /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Bouton s\303\251lectionn\303\251 */\n"
"QPushButton:checked {\n"
"    background-color: #2A2DAC;  /* Bleu plus fonc\303\251 pour bouton s\303\251lectionn\303\251 */\n"
"    color: #FFFFFF;              /* Texte en blanc */\n"
"    border: 3px solid #353FF2;   /* Bordure bleu vif */\n"
"    border-radius: 12px;         /* Coins arrondis */\n"
"    padding: 8px 12px;           /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 18"
                        "pt \"Arial\";       /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #4A5BCC;   /* Bleu clair au survol */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0; /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070; /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0; /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
""));
        pushButton_11 = new QPushButton(centralwidget);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setGeometry(QRect(-40, 540, 261, 51));
        pushButton_11->setStyleSheet(QString::fromUtf8("/* Bouton non s\303\251lectionn\303\251 */\n"
"QPushButton {\n"
"    background-color: #5cb5f2;  /* Bleu vif pour un look attrayant */\n"
"    color: #FFFFFF;             /* Texte en blanc */\n"
"    border: 3px solid #2A2DAC;  /* Bordure bleu fonc\303\251 pour un bon contraste */\n"
"    border-radius: 12px;        /* Coins arrondis */\n"
"    padding: 8px 12px;          /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 15pt \"Arial\";      /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Bouton s\303\251lectionn\303\251 */\n"
"QPushButton:checked {\n"
"    background-color: #2A2DAC;  /* Bleu plus fonc\303\251 pour bouton s\303\251lectionn\303\251 */\n"
"    color: #FFFFFF;              /* Texte en blanc */\n"
"    border: 3px solid #353FF2;   /* Bordure bleu vif */\n"
"    border-radius: 12px;         /* Coins arrondis */\n"
"    padding: 8px 12px;           /* Espacement interne, augment\303\251 pour plus de confort */\n"
"    font: 75 18"
                        "pt \"Arial\";       /* Police en gras, taille 18pt pour une meilleure lisibilit\303\251 */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est survol\303\251 */\n"
"QPushButton:hover {\n"
"    background-color: #4A5BCC;   /* Bleu clair au survol */\n"
"}\n"
"\n"
"/* Style du QPushButton lorsqu'il est d\303\251sactiv\303\251 */\n"
"QPushButton:disabled {\n"
"    background-color: #b0b0b0; /* Fond gris clair pour les boutons d\303\251sactiv\303\251s */\n"
"    color: #707070; /* Couleur du texte grise pour les boutons d\303\251sactiv\303\251s */\n"
"    border: 2px solid #b0b0b0; /* Bordure grise pour les boutons d\303\251sactiv\303\251s */\n"
"}\n"
""));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1444, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Gestion Emply\303\251es", nullptr));
        label_19->setText(QString());
        label_20->setText(QString());
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "Gestion Reclamation", nullptr));
        label_17->setText(QString());
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "liste des locaux", nullptr));
        lineedit_id_rech->setPlaceholderText(QCoreApplication::translate("MainWindow", "Rechercher par ID", nullptr));
        label_49->setText(QCoreApplication::translate("MainWindow", "Rechercher le local \303\240 supprimer par ID", nullptr));
        label_48->setText(QCoreApplication::translate("MainWindow", "ID_local:", nullptr));
        supprimerButton->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        pb_pdf->setText(QCoreApplication::translate("MainWindow", "PDF selon local disponible", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "Rechercher un local par ID:", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "Trier la liste des local par prix:", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Afficher les statistiques des locaux par disponibiliter selon moin:", nullptr));
        rechercherButton->setText(QCoreApplication::translate("MainWindow", "Rechercher", nullptr));
        trierButton->setText(QCoreApplication::translate("MainWindow", "Trier", nullptr));
        statistiqueButton->setText(QCoreApplication::translate("MainWindow", "statistiques", nullptr));
        statistiqueButton_2->setText(QCoreApplication::translate("MainWindow", "statistiques par graphique", nullptr));
        pb_historiqueService->setText(QCoreApplication::translate("MainWindow", "Historique", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(afficher), QCoreApplication::translate("MainWindow", "Liste", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "AJOUTER", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "adresse:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "vile:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Type de local:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Disponibiliter:", nullptr));
        l2->setText(QCoreApplication::translate("MainWindow", "etrez la vile", nullptr));
        l2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez le prenom du service ", nullptr));
        l4->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez la disponibilit\303\251", nullptr));
        ajouterButton->setText(QCoreApplication::translate("MainWindow", "Ajouter ", nullptr));
        l1->setText(QCoreApplication::translate("MainWindow", "entrez type de local", nullptr));
        l1->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez  l'idantifiant", nullptr));
        l3->setText(QCoreApplication::translate("MainWindow", "Etrez l'adresse", nullptr));
        l3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez le prix", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Disponible 1", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "non Disponible 0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "nom de proprietaire", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "numero", nullptr));
        l4_2->setText(QCoreApplication::translate("MainWindow", "Entrez le nom de proprietaire", nullptr));
        l4_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez la disponibilit\303\251", nullptr));
        l4_3->setText(QCoreApplication::translate("MainWindow", "Etrez le num\303\251ro", nullptr));
        l4_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez la disponibilit\303\251", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "prix", nullptr));
        l4_4->setText(QCoreApplication::translate("MainWindow", "Etrez le prix", nullptr));
        l4_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez la disponibilit\303\251", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Cin_de proprietaire", nullptr));
        l4_5->setText(QCoreApplication::translate("MainWindow", "Etrez le num\303\251ro", nullptr));
        l4_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez la disponibilit\303\251", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Ajouter), QCoreApplication::translate("MainWindow", "Ajouter+ouverture", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "MODIFIER", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Prix:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Disponibilit\303\251 :", nullptr));
        l8->setPlaceholderText(QCoreApplication::translate("MainWindow", "Modifier la disponibilit\303\251 ", nullptr));
        modifierButton->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        lineedit_id_modif->setPlaceholderText(QCoreApplication::translate("MainWindow", "Modifier par ID", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "ID_local:", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "donner le local \303\240 modifier par ID", nullptr));
        l7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Modifier le prix", nullptr));
        label_46->setText(QCoreApplication::translate("MainWindow", "1:Disponible", nullptr));
        label_47->setText(QCoreApplication::translate("MainWindow", "0:Non disponible", nullptr));
        sendSMSButton->setText(QCoreApplication::translate("MainWindow", "SMS", nullptr));
        contenu->setPlainText(QString());
        tabWidget->setTabText(tabWidget->indexOf(Modifier), QCoreApplication::translate("MainWindow", "modifier", nullptr));
        label_14->setText(QString());
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Gestion Ressource", nullptr));
        label_18->setText(QString());
        pushButton_menu->setText(QCoreApplication::translate("MainWindow", "Gestion local", nullptr));
        pushButton_11->setText(QCoreApplication::translate("MainWindow", "Gestion R\303\251sident", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
