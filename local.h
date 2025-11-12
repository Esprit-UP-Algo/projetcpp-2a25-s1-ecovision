#ifndef LOCAL_H
#define LOCAL_H

#include <QString>
#include <QSqlDatabase>
#include <QVariant>
#include <optional>

class Local
{
public:
    QString type_local;
    QString ville;
    QString adresse;
    int     disponibilite;
    QString nom_proprietaire;
    QString cin_proprietaire;
    QString numero;
    double  prix;

    Local(const QString& typeLocal = {},
          const QString& ville = {},
          const QString& adresse = {},
          int disponibilite = 0,
          const QString& nomProprietaire = {},
          const QString& cinProprietaire = {},
          const QString& numero = {},
          double prix = 0.0);

    bool isValid(QString* errorMessage = nullptr) const;

    bool insert(QString* errorMessage = nullptr,
                QSqlDatabase db = QSqlDatabase()) const;

    static bool ensureEmployeExists(const QString& cin,
                                    QString* errorMessage = nullptr,
                                    QSqlDatabase db = QSqlDatabase());

    static bool fetchPrixDispo(int id_local,
                               QVariant* prix,
                               QVariant* dispo,
                               QString* errorMessage = nullptr,
                               QSqlDatabase db = QSqlDatabase());

    static bool updateById(int id_local,
                           const std::optional<double>& newPrix,
                           const std::optional<int>& newDispo,
                           QString* errorMessage = nullptr,
                           QSqlDatabase db = QSqlDatabase());

    static bool deleteById(int id_local,
                           QString* errorMessage = nullptr,
                           QSqlDatabase db = QSqlDatabase());
};

#endif // LOCAL_H
