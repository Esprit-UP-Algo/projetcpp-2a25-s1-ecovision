#include "import.h"
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

Import::Import(QObject *parent) : QObject(parent) {}

void Import::importDatabase(QWidget *parentWidget) {
    // Open file dialog to select CSV file
    QString fileName = QFileDialog::getOpenFileName(parentWidget, "Open Backup File", "", "CSV Files (*.csv)");
    if (fileName.isEmpty()) return;

    // Parse the CSV file
    QStringList data = parseCSVFile(fileName);
    if (data.isEmpty()) {
        QMessageBox::critical(parentWidget, "Error", "Failed to read the backup file or file is empty.");
        return;
    }


    // Insert data into the database
    if (insertIntoDatabase(data)) {
        QMessageBox::information(parentWidget, "Success", "Data imported successfully.");
    } else {
        QMessageBox::warning(parentWidget, "Error", "Failed to insert data into the database.");
    }
}

QStringList Import::parseCSVFile(const QString &fileName) {
    QFile file(fileName);
    QStringList rows;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File Error: Cannot open file for reading.";
        return rows;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        rows.append(line);
    }

    file.close();
    return rows;
}



bool Import::insertIntoDatabase(const QStringList &data) {
    if (data.isEmpty()) return false;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        qDebug() << "Database connection is invalid or not open.";
        return false;
    }

    QSqlQuery query;
    QString header = data.first();
    QStringList columns = header.split(',');

    if (columns.size() != 7) {
        qDebug() << "Error: CSV file does not have the correct column count.";
        return false;
    }

    for (int i = 1; i < data.size(); ++i) {
        QStringList values = data[i].split(',');
        if (values.size() != columns.size()) {
            qDebug() << "Error: Data row column count does not match the header.";
            continue;
        }

        // Convert numeric fields
        int id = values[0].toInt();
        int capacite = values[3].toInt();
        int placesDisponibles = values[4].toInt();
        double tarifHoraire = values[5].toDouble();

        // Prepare query
        query.prepare("INSERT INTO PARKING (ID, NOM, EMPLACEMENT, CAPACITE, PLACES_DISPONIBLES, TARIF_HORAIRE, STATUT) "
                      "VALUES (:id, :nom, :emplacement, :capacite, :placesDisponibles, :tarifHoraire, :statut)");

        // Bind values correctly
        query.bindValue(":id", id);
        query.bindValue(":nom", values[1]);
        query.bindValue(":emplacement", values[2]);
        query.bindValue(":capacite", capacite);
        query.bindValue(":placesDisponibles", placesDisponibles);
        query.bindValue(":tarifHoraire", tarifHoraire);
        query.bindValue(":statut", values[6]);

        if (!query.exec()) {
            qDebug() << "Database Insertion Error:" << query.lastError().text() << "Row:" << i;
        }
    }

    return true;
}
