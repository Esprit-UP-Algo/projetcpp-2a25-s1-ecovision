#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load Inter font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/roboto.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);

    QFont appFont(family, 11); // 11pt default
    a.setFont(appFont);

    MainWindow w;
    w.show();
    return a.exec();
}
