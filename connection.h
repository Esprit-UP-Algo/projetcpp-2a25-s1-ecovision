#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>

namespace Ui {
class connection;
}

class connection : public QWidget
{
    Q_OBJECT

public:
    explicit connection(QWidget *parent = nullptr);
    ~connection();

signals:
    void loginSucceeded();

private slots:
    void onLoginClicked();

private:
    Ui::connection *ui;
};

#endif // CONNECTION_H
