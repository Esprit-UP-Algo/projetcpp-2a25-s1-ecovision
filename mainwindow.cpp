#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resourcewindow.h"
#include "localwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set window title
    setWindowTitle("Gestion de Résidence");

    // Connect navigation buttons
    connect(ui->pushButton_ressources, &QPushButton::clicked, this, &MainWindow::on_pushButton_ressources_clicked);
    connect(ui->pushButton_2_ressources, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_ressources_clicked);
    connect(ui->pushButton_3_ressources, &QPushButton::clicked, this, &MainWindow::on_pushButton_3_ressources_clicked);
    connect(ui->pushButton_4_ressources, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_ressources_clicked);
    connect(ui->pushButton_5_ressources, &QPushButton::clicked, this, &MainWindow::on_pushButton_5_ressources_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_ressources_clicked()
{
    QMessageBox::information(this, "Info", "Gestion Employé - À implémenter");
}

void MainWindow::on_pushButton_2_ressources_clicked()
{
    qDebug() << "Opening LocalWindow...";

    // Use stack allocation to avoid memory issues
    LocalWindow localDialog(this);
    localDialog.setModal(true);
    localDialog.exec();

    qDebug() << "LocalWindow closed";
}

void MainWindow::on_pushButton_3_ressources_clicked()
{
    QMessageBox::information(this, "Info", "Gestion Resident - À implémenter");
}

void MainWindow::on_pushButton_4_ressources_clicked()
{
    QMessageBox::information(this, "Info", "Gestion Reclamation - À implémenter");
}

void MainWindow::on_pushButton_5_ressources_clicked()
{
    qDebug() << "Opening ResourceWindow...";

    // Use stack allocation to avoid memory issues
    ResourceWindow resourceDialog(this);
    resourceDialog.setModal(true);
    resourceDialog.exec();

    qDebug() << "ResourceWindow closed";
}
