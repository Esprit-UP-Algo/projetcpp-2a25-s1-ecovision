#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>
#include <QSqlDatabase>

struct Connection {
    static bool open(QString* err = nullptr);
    static QSqlDatabase db() { return QSqlDatabase::database(); }
};

#endif // CONNECTION_H
