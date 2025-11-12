#include "reclamation.h"

Reclamation::Reclamation()
{
    code = 0;
    date_reclamation = QDate::currentDate();
    nom_resident = "";
    description = "";
    etat = "";
    delai = "";
}

Reclamation::Reclamation(int c, QDate d, QString n, QString desc, QString e, QString del)
{
    code = c;
    date_reclamation = d;
    nom_resident = n;
    description = desc;
    etat = e;
    delai = del;
}

bool Reclamation::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO RECLAMATION (DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI) "
                  "VALUES (:date_rec, :nom, :desc, :etat, :delai)");

    query.bindValue(":date_rec", date_reclamation);
    query.bindValue(":nom", nom_resident);
    query.bindValue(":desc", description);
    query.bindValue(":etat", etat);
    query.bindValue(":delai", delai);

    return query.exec();
}

QSqlQueryModel* Reclamation::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT CODE, DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI FROM RECLAMATION ORDER BY CODE DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Code"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Date Réclamation"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Nom Résident"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Délai"));

    return model;
}

bool Reclamation::supprimer(int code)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RECLAMATION WHERE CODE = :code");
    query.bindValue(":code", code);
    return query.exec();
}
// ==================== MODIFIER ====================
bool Reclamation::modifier(int code)
{
    QSqlQuery query;
    query.prepare("UPDATE RECLAMATION SET DATE_RECLAMATION = :date_rec, "
                  "NOM_RESIDENT = :nom, DESCRIPTION = :desc, "
                  "ETAT = :etat, DELAI = :delai "
                  "WHERE CODE = :code");

    query.bindValue(":code", code);
    query.bindValue(":date_rec", date_reclamation);
    query.bindValue(":nom", nom_resident);
    query.bindValue(":desc", description);
    query.bindValue(":etat", etat);
    query.bindValue(":delai", delai);

    return query.exec();
}

// ==================== RECHERCHER PAR CODE ====================
Reclamation Reclamation::rechercherParCode(int code)
{
    QSqlQuery query;
    query.prepare("SELECT CODE, DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI "
                  "FROM RECLAMATION WHERE CODE = :code");
    query.bindValue(":code", code);

    if(query.exec() && query.next())
    {
        return Reclamation(
            query.value(0).toInt(),
            query.value(1).toDate(),
            query.value(2).toString(),
            query.value(3).toString(),
            query.value(4).toString(),
            query.value(5).toString()
            );
    }

    return Reclamation(); // Retourne une réclamation vide si non trouvée
}

// ==================== RECHERCHER PAR DATE ====================
QList<Reclamation> Reclamation::rechercherParDate(QDate date)
{
    QList<Reclamation> liste;
    QSqlQuery query;

    query.prepare("SELECT CODE, DATE_RECLAMATION, NOM_RESIDENT, DESCRIPTION, ETAT, DELAI "
                  "FROM RECLAMATION WHERE DATE_RECLAMATION = :date "
                  "ORDER BY CODE DESC");
    query.bindValue(":date", date);

    if(query.exec())
    {
        while(query.next())
        {
            Reclamation r(
                query.value(0).toInt(),
                query.value(1).toDate(),
                query.value(2).toString(),
                query.value(3).toString(),
                query.value(4).toString(),
                query.value(5).toString()
                );
            liste.append(r);
        }
    }

    return liste;
}
