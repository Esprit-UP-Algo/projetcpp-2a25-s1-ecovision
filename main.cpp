#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "connectdb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    connection c;
    connectDb db;
    bool test=db.createconnect();
    if(!test)
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);

    QObject::connect(&c, &connection::loginSucceeded, [&]() {
        w.show();
        w.onDisplayAllEmployes();
        c.close();
    });

    c.show();
    return a.exec();
}
