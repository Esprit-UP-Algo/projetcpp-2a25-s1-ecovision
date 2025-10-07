#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString cheminLogo = "C:/Users/amine/OneDrive/Images/logo4.png";
    QPixmap logo(cheminLogo);
    ui->image->setPixmap(logo.scaled(
        ui->image->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        ));
}

MainWindow::~MainWindow()
{
    delete ui;
}
