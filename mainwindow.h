#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Forward declarations
class ResourceWindow;
class LocalWindow;  // Add this line

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_ressources_clicked();
    void on_pushButton_2_ressources_clicked();  // Local button
    void on_pushButton_3_ressources_clicked();
    void on_pushButton_4_ressources_clicked();
    void on_pushButton_5_ressources_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
