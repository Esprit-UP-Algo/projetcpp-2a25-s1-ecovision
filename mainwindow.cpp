#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"

#include "import.h"
#include "backup.h"

// for pdf
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

// statis
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QPixmap>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>


#include "QSqlError"
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include "QRandomGenerator"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadParkingsIntoCombo();
    initArduino();
    connect(ui->exportPdf, &QPushButton::clicked, this, &MainWindow::on_exportPdf_clicked);
    connect(ui->generateQrCodes, &QPushButton::clicked, this, &MainWindow::generateQrCodes);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouterclient_clicked()
{

    int id = ui->id->text().toInt();
    QString nom = ui->nom->text().trimmed();
    QString emplacement = ui->emplacement->text().trimmed();
    int capacite = ui->capacite->text().toInt();
    int placesDisponibles = ui->placesDisponibles->text().toInt();
    double tarifHoraire = ui->tariff->text().toDouble();
    QString statut = ui->statut->currentText().trimmed();

    qDebug() << "ID:" << id;
    qDebug() << "Nom:" << nom;
    qDebug() << "Emplacement:" << emplacement;
    qDebug() << "Capacité:" << capacite;
    qDebug() << "Places Disponibles:" << placesDisponibles;
    qDebug() << "Tarif Horaire:" << tarifHoraire;
    qDebug() << "Statut:" << statut;

    if (nom.isEmpty() || emplacement.isEmpty() || statut.isEmpty()) {
        QMessageBox::critical(this, tr("ERREUR"), tr("Veuillez remplir tous les champs obligatoires."), QMessageBox::Cancel);
        return;
    }

    Parking parking(id, nom, emplacement, capacite, placesDisponibles, tarifHoraire, statut);

    if (parking.ajouter()) {
        QMessageBox::information(this, tr("Succès"), tr("Parking ajouté avec succès."));
        ui->tableView->setModel(parking.afficher());  // Mise à jour du tableau
        loadParkingsIntoCombo();

    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de l'ajout du parking. ID peut-être déjà utilisé."));
    }
}

void MainWindow::on_modifierclient_clicked()
{
    int id = ui->id->text().toInt();
    QString nom = ui->nom->text().trimmed();
    QString emplacement = ui->emplacement->text().trimmed();
    int capacite = ui->capacite->text().toInt();
    int placesDisponibles = ui->placesDisponibles->text().toInt();
    double tarifHoraire = ui->tariff->text().toDouble();
    QString statut = ui->statut->currentText().trimmed();

    if (nom.isEmpty() || emplacement.isEmpty() || statut.isEmpty()) {
        QMessageBox::critical(this, tr("Erreur"), tr("Veuillez remplir tous les champs obligatoires."), QMessageBox::Cancel);
        return;
    }

    Parking parking(id, nom, emplacement, capacite, placesDisponibles, tarifHoraire, statut);

    // Vérifier si l'ID existe avant modification
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM PARKING WHERE ID = :id");
    checkQuery.bindValue(":id", id);
    checkQuery.exec();
    checkQuery.next();
    if (checkQuery.value(0).toInt() == 0) {
        QMessageBox::critical(this, tr("Erreur"), tr("L'ID spécifié n'existe pas."));
        return;
    }

    bool test = parking.mettreAJour(id);

    if (test) {
        ui->tableView->setModel(parking.afficher());  // actualisation du tableau
        QMessageBox::information(this, tr("Succès"), tr("Parking modifié avec succès."));
        loadParkingsIntoCombo();

    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la modification du parking."));
    }
}



void MainWindow::on_supprclient_clicked()
{
    int id = ui->suppc->text().toInt(); // Récupérer le id depuis le champ input
    Parking parking;

    if (!parking.exists(id)) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Le ID spécifié n'existe pas!"));
        return;
    }

    bool test = parking.supprimer(id);

    if (test) {
        ui->tableView->setModel(parking.afficher()); // Rafraîchir la table
        QMessageBox::information(nullptr, QObject::tr("Succès"), tr("Suppression effectuée avec succès!"));
        loadParkingsIntoCombo();

    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Suppression non effectuée!"));
    }
}


void MainWindow::on_exportButton_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF", "", "PDF Files (*.pdf)");

    if (fileName.isEmpty()) {
        return;
    }


    QTextDocument doc;


    QString htmlContent;


    htmlContent += "<h1 style='text-align: center;'>Parking List</h1>";
    htmlContent += "<hr/>";


    htmlContent += "<table border='1' cellpadding='7' cellspacing='0' width='100%'>";


    htmlContent += "<tr>"
                   "<th>ID</th>"
                   "<th>Name</th>"
                   "<th>Emplacement</th>"
                   "<th>Capacite</th>"
                   "<th>PlacesDisponnibles</th>"
                   "<th>Tarif</th>"
                   "<th>Statut</th>"

                   "</tr>";


    QAbstractItemModel *model = ui->tableView->model();


    for (int row = 0; row < model->rowCount(); ++row) {
        htmlContent += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            QString cellData = model->data(model->index(row, col)).toString();
            htmlContent += "<td>" + cellData + "</td>";
        }
        htmlContent += "</tr>";
    }

    htmlContent += "</table>";


    doc.setHtml(htmlContent);


    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));


    doc.print(&printer);


    QMessageBox::information(this, "Export Successful", "The data has been exported to PDF.");
}
void MainWindow::on_backup_clicked() {
    Backup backup;
    backup.exportDatabase(this);
}

void MainWindow::on_import_2_clicked() {
    Import importer;
    importer.importDatabase(this);

    ui->tableView->setModel(parking.afficher());
}
void MainWindow::on_afficherclient_clicked()
{
    Parking p; // créer un objet temporaire
    ui->tableView->setModel(p.afficher()); // affecter le modèle au tableau
}
void MainWindow::on_triId_clicked()
{
    Parking p;
    ui->tableView->setModel(p.trierParId());
}
void MainWindow::on_triNom_clicked()
{
    Parking p;
    ui->tableView->setModel(p.trierParNom());
}
void MainWindow::on_triCapacite_clicked()
{
    Parking p;
    ui->tableView->setModel(p.trierParCapacite());
}
void MainWindow::on_rechclient_2_clicked()
{
    int id = ui->rechclient->text().toInt();  // champ texte où tu entres l'ID
    Parking p;

    // Vérifier si l'ID existe
    if (!p.exists(id)) {
        QMessageBox::warning(this, "Recherche", "Aucun parking trouvé pour cet ID !");
        return;
    }

    // Sinon afficher le résultat dans le tableau
    ui->tableView->setModel(p.rechercher(id));
}
void MainWindow::on_exportPdf_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF (*.pdf)");
    if (fileName.isEmpty())
        return;

    QTextDocument doc;

    // Récupérer les données du TableView
    QAbstractItemModel *model = ui->tableView->model();

    // ------------------------
    //         HTML DESIGN
    // ------------------------
    QString html = R"(
        <html>
        <head>
        <style>
            body {
                font-family: Arial, Helvetica, sans-serif;
                margin: 20px;
            }
            h1 {
                text-align: center;
                color: #2c3e50;
                font-size: 200px;
                margin-bottom: 20px;
            }
            table {
                width: 100%;
                border-collapse: collapse;
                margin-top: 15px;
            }
            th {
                background-color: #34495e;
                color: white;
                padding: 10px;
                text-align: center;
                font-size: 200px;
            }
            td {
                border: 1px solid #bdc3c7;
                padding: 8px;
                text-align: center;
                font-size: 200px;
            }
            tr:nth-child(even) {
                background-color: #ecf0f1;
            }
            tr:hover {
                background-color: #dfe6e9;
            }
        </style>
        </head>
        <body>
        <h1>Liste des Parkings</h1>
        <table>
            <tr>
    )";

    // Ajouter les Headers
    for (int col = 0; col < model->columnCount(); ++col) {
        html += "<th>" + model->headerData(col, Qt::Horizontal).toString() + "</th>";
    }
    html += "</tr>";

    // Ajouter les lignes du tableau
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            QString data = model->data(model->index(row, col)).toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }

    // Fin HTML
    html += R"(
        </table>
        </body>
        </html>
    )";

    doc.setHtml(html);

    // Printer PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    doc.print(&printer);

    QMessageBox::information(this, "PDF Exporté", "La liste des parkings a été exportée avec succès !");
}

#include "qrcodegen.hpp"
using namespace qrcodegen;

#include <QImage>
#include <QDir>
#include <QPainter>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

void MainWindow::generateQrCodes()
{
    // Folder to save QR codes
    QDir dir("generated_qrcodes");
    if (!dir.exists()) dir.mkpath(".");

    // Query all parkings from the database
    QSqlQuery query("SELECT ID, NOM, EMPLACEMENT, CAPACITE, PLACES_DISPONIBLES, TARIF_HORAIRE, STATUT FROM PARKING");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString nom = query.value(1).toString();
        QString emplacement = query.value(2).toString();
        int capacite = query.value(3).toInt();
        int placesDispo = query.value(4).toInt();
        double tarif = query.value(5).toDouble();
        QString statut = query.value(6).toString();

        // Create QR content with all attributes
        QString content = QString(
                              "ID: %1\nName: %2\nEmplacement: %3\nCapacite: %4\nPlacesDisponibles: %5\nTarifHoraire: %6\nStatut: %7"
                              ).arg(id).arg(nom).arg(emplacement).arg(capacite).arg(placesDispo).arg(tarif).arg(statut);

        // Generate QR code
        QrCode qr = QrCode::encodeText(content.toUtf8().constData(), QrCode::Ecc::MEDIUM);
        int size = qr.getSize();
        QImage qrImage(size, size, QImage::Format_RGB32);

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                qrImage.setPixel(x, y, qr.getModule(x, y) ? qRgb(0,0,0) : qRgb(255,255,255));
            }
        }

        // Scale QR image
        qrImage = qrImage.scaled(200, 200);

        // Create final image with ID & Name below
        QImage finalImage(200, 230, QImage::Format_RGB32);
        finalImage.fill(Qt::white);
        QPainter painter(&finalImage);
        painter.drawImage(0, 0, qrImage);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 12));
        painter.drawText(finalImage.rect().adjusted(0,200,0,0), Qt::AlignCenter,
                         QString("ID: %1\n%2").arg(id).arg(nom));
        painter.end();

        // Save QR code
        QString fileName = dir.filePath(QString("parking_%1.png").arg(id));
        finalImage.save(fileName);
        qDebug() << "Saved QR code for parking ID" << id << "at" << fileName;
    }

    QMessageBox::information(this, "QR Codes", "All QR codes generated in 'generated_qrcodes' folder.");
}
void MainWindow::readSerial()
{
    // read_from_arduino() is the method from your Arduino class
    data = A.read_from_arduino();

    if (!data.isEmpty()) {
        qDebug() << "Data received from Arduino:" << data;
    }
}

void MainWindow::loadParkingsIntoCombo()
{
    ui->comboParking->clear();

    QSqlQuery q("SELECT ID, NOM FROM PARKING ORDER BY NOM");

    while (q.next()) {
        int id = q.value(0).toInt();
        QString nom = q.value(1).toString();

        // text shown = NOM, userData = ID
        ui->comboParking->addItem(nom, id);
    }
}
void MainWindow::initArduino()
{
    qDebug() << "initArduino() called";

    int ret = A.connect_arduino();          // calls your Arduino::connect_arduino()

    qDebug() << "A.connect_arduino() returned:" << ret;

    if (ret == 0) {
        qDebug() << "Arduino connected on port:" << A.getarduino_port_name();

        // if you want to read data later:
        QObject::connect(A.getserial(), &QSerialPort::readyRead,
                         this, &MainWindow::readSerial);
    } else {
        qDebug() << "Arduino not connected (ret =" << ret << ")";
        // optionally also show a QMessageBox if you want
        // QMessageBox::warning(this, "Arduino", "Arduino not connected");
    }
}

void MainWindow::on_btnEnvoyerEtat_clicked()
{
        // 1) Arduino must be open
        if (!A.getserial() || !A.getserial()->isOpen()) {
            qDebug() << "Arduino not connected";
            return;
        }

        // 2) Get selected parking from combo
        int idx = ui->comboParking->currentIndex();
        if (idx < 0) {
            qDebug() << "No parking selected in comboBox";
            return;
        }

        int parkingId = ui->comboParking->itemData(idx).toInt();
        QString parkingName = ui->comboParking->currentText();

        // 3) Read STATUT of that parking from DB
        QSqlQuery q;
        q.prepare("SELECT STATUT FROM PARKING WHERE ID = :id");
        q.bindValue(":id", parkingId);

        if (!q.exec() || !q.next()) {
            qDebug() << "Error reading STATUT for parking ID" << parkingId
                     << q.lastError().text();
            return;
        }

        QString statut = q.value(0).toString().toUpper().trimmed();
        bool isFree = (statut == "DISPONIBLE" || statut == "DISPO" || statut == "FREE");

        // 4) Send command to Arduino
        QByteArray cmd = isFree ? "SET:1\n" : "SET:0\n";
        A.write_to_arduino(cmd);

        int etatValue = isFree ? 1 : 0;

        qDebug() << "Parking" << parkingName
                 << "(ID =" << parkingId << ") statut =" << statut
                 << "-> send" << cmd.trimmed() << "etat =" << etatValue;

        // 5) Log into PARKING_ARDUINO_LOG with random ID_LOG and current timestamp
        int idLog = QRandomGenerator::global()->bounded(1, 1000000000);

        QSqlQuery log;
        log.prepare("INSERT INTO PARKING_ARDUINO_LOG "
                    "(ID_LOG, ID_PARKING, ETAT, LOG_TIME) "
                    "VALUES (:id_log, :id_parking, :etat, SYSTIMESTAMP)");

        log.bindValue(":id_log",     idLog);
        log.bindValue(":id_parking", parkingId);
        log.bindValue(":etat",       etatValue);

        if (!log.exec()) {
            qDebug() << "Error inserting into PARKING_ARDUINO_LOG:"
                     << log.lastError().text();
        } else {
            qDebug() << "Inserted log row: ID_LOG =" << idLog
                     << "ID_PARKING =" << parkingId
                     << "ETAT =" << etatValue;
        }
}
