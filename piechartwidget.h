#ifndef PIECHARTWIDGET_H
#define PIECHARTWIDGET_H

#include <QWidget>
#include <QMap>

class PieChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PieChartWidget(QWidget *parent = nullptr);
    void setData(const QMap<QString, int>& data, const QMap<QString, QString>& colors);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMap<QString, int> m_data;
    QMap<QString, QString> m_colors;
    int m_total;
};

#endif // PIECHARTWIDGET_H
