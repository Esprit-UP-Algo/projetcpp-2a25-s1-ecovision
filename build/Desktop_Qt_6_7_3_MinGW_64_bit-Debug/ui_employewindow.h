/********************************************************************************
** Form generated from reading UI file 'employewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYEWINDOW_H
#define UI_EMPLOYEWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
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
    QTableWidget *tableWidget;
    QLineEdit *cin_4;
    QLabel *cin_3;
    QLabel *presence;
    QLineEdit *presence_2;
    QPushButton *btnpdf;
    QLabel *salaire;
    QLineEdit *salaire_2;
    QWidget *tabStat;
    QChartView *stat;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName("Dialog");
        Dialog->resize(1377, 856);
        rightContent = new QFrame(Dialog);
        rightContent->setObjectName("rightContent");
        rightContent->setGeometry(QRect(20, 50, 1191, 761));
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
        tableWidget = new QTableWidget(tabCRUD);
        if (tableWidget->columnCount() < 8)
            tableWidget->setColumnCount(8);
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
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(-10, 90, 561, 441));
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

        retranslateUi(Dialog);

        CRUD_Stat->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Dialog", nullptr));
        titleLabel->setText(QCoreApplication::translate("Dialog", "Gestion Employ\303\251e", nullptr));
        btnAjouter->setText(QCoreApplication::translate("Dialog", "Ajouter", nullptr));
        btnModifier->setText(QCoreApplication::translate("Dialog", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("Dialog", "Supprimer", nullptr));
        nom->setText(QCoreApplication::translate("Dialog", "nom:", nullptr));
        adresse->setText(QCoreApplication::translate("Dialog", "adresse:", nullptr));
        cin->setText(QCoreApplication::translate("Dialog", "cin:", nullptr));
        telephone->setText(QCoreApplication::translate("Dialog", "telephone:", nullptr));
        prenom->setText(QCoreApplication::translate("Dialog", "prenom:", nullptr));
        poste->setText(QCoreApplication::translate("Dialog", "poste", nullptr));
        btntrier->setText(QCoreApplication::translate("Dialog", "trier", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("Dialog", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("Dialog", "NOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("Dialog", "PRENOM", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("Dialog", "TELEPHONE", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("Dialog", "ADRESSE", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("Dialog", "POSTE", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("Dialog", "PRESENCE", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("Dialog", "SALAIRE", nullptr));
        cin_4->setPlaceholderText(QString());
        cin_3->setText(QCoreApplication::translate("Dialog", "Saisir le cin \303\240 chercher : ", nullptr));
        presence->setText(QCoreApplication::translate("Dialog", "presence:", nullptr));
        btnpdf->setText(QCoreApplication::translate("Dialog", "pdf", nullptr));
        salaire->setText(QCoreApplication::translate("Dialog", "salaire:", nullptr));
        CRUD_Stat->setTabText(CRUD_Stat->indexOf(tabCRUD), QCoreApplication::translate("Dialog", "Inforamtions", nullptr));
        CRUD_Stat->setTabText(CRUD_Stat->indexOf(tabStat), QCoreApplication::translate("Dialog", "Statistiques", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYEWINDOW_H
