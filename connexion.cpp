#include "connexion.h"
#include "ui_connexion.h"

connexion::connexion(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::connexion)
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

connexion::~connexion()
{
    delete ui;
}
