#ifndef CONNECTION_H
#define CONNECTION_H

#include <QString>

class Connection {
public:
    static bool open(QString* err = nullptr);
};

#endif // CONNECTION_H
