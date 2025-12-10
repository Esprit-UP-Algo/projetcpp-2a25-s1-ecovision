#include "piechartwidget.h"
#include <QPainter>
#include <QDebug>

PieChartWidget::PieChartWidget(QWidget *parent)
    : QWidget(parent), m_total(0)
{
    setFixedSize(100, 100);
}

void PieChartWidget::setData(const QMap<QString, int>& data, const QMap<QString, QString>& colors)
{
    m_data = data;
    m_colors = colors;
    m_total = 0;

    for (int count : data.values()) {
        m_total += count;
    }

    qDebug() << "PieChartWidget: Received data - Total:" << m_total << "Items:" << data.size();
    qDebug() << "PieChartWidget: Colors available:" << colors;

    update(); // Trigger repaint
}

void PieChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw background circle
    painter.setBrush(QColor("#f1f5f9"));
    painter.setPen(QPen(QColor("#e2e8f0"), 2));
    painter.drawEllipse(2, 2, width()-4, height()-4);

    if (m_total == 0) {
        // Draw "No Data" text
        painter.setPen(QColor("#94a3b8"));
        painter.drawText(rect(), Qt::AlignCenter, "No Data");
        return;
    }

    // Draw pie chart
    QRectF rect(2, 2, width()-4, height()-4);
    double currentAngle = 0;

    qDebug() << "PieChartWidget: Drawing pie chart with" << m_data.size() << "segments";

    int segmentIndex = 0;
    for (const QString& key : m_data.keys()) {
        if (m_data[key] > 0) {
            double angle = (m_data[key] * 360.0) / m_total;

            QString colorName = m_colors.value(key, "#94a3b8");
            QColor color(colorName);

            qDebug() << "PieChartWidget: Segment" << segmentIndex << "Key:" << key
                     << "Count:" << m_data[key] << "Angle:" << angle << "Color:" << colorName;

            painter.setBrush(color);
            painter.setPen(QPen(QColor("#e2e8f0"), 1));

            painter.drawPie(rect, currentAngle * 16, angle * 16);

            currentAngle += angle;
            segmentIndex++;
        }
    }
}
