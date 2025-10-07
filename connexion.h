#ifndef CONNEXION_H
#define CONNEXION_H

#include <QWidget>

namespace Ui {
class connexion;
}

class connexion : public QWidget
{
    Q_OBJECT

public:
    explicit connexion(QWidget *parent = nullptr);
    ~connexion();

private:
    Ui::connexion *ui;
};

#endif // CONNEXION_H
