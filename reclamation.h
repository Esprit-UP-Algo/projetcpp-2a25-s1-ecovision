#pragma once
#include <QString>
#include <QDate>
#include <QSqlDatabase>

class Reclamation {
public:
    Reclamation(int code,
                const QDate& dateReclam,
                const QString& nomResident,
                const QString& description,
                const QString& etat,
                const QString& delai);

    bool isValid(QString* err = nullptr) const;
    bool insert(QString* err = nullptr, QSqlDatabase db = QSqlDatabase()) const;

private:
    int code_;
    QDate dateReclam_;
    QString nomResident_;
    QString description_;
    QString etat_;
    QString delai_;
};
