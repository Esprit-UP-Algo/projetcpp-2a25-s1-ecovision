#ifndef STATWINDOW_H
#define STATWINDOW_H

#include <QDialog>
#include <QPropertyAnimation>
#include <QVector>
#include <QLabel>

class StatWindow : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(double progress READ progress WRITE setProgress)

public:
    explicit StatWindow(QWidget *parent = nullptr);

    double progress() const { return m_progress; }
    void setProgress(double value);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void loadDataFromDatabase();
    void drawLegend(QPainter &p, int startY);

    double m_progress;
    QPropertyAnimation *anim;

    QVector<double> values;
    QVector<QColor> colors;
    QVector<QString> labels;
};

#endif // STATWINDOW_H
