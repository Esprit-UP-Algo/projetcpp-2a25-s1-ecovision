#include "StatWindow.h"
#include "resource.h"
#include <QPainter>
#include <QtMath>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

StatWindow::StatWindow(QWidget *parent)
    : QDialog(parent), m_progress(0)
{
    setFixedSize(500, 500); // Increased size for legend
    setWindowTitle("Statistiques de Consommation");

    // Define colors and labels
    colors = { QColor(65, 105, 225),   // Royal Blue for water
              QColor(220, 20, 60),    // Crimson for electricity
              QColor(46, 139, 87) };  // Sea Green for other

    labels = { "Eau", "Électricité", "Autres" };

    // Load real data from database
    loadDataFromDatabase();

    // Animation
    anim = new QPropertyAnimation(this, "progress");
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setDuration(1500);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start();
}

void StatWindow::loadDataFromDatabase()
{
    // Method 1: Using the static method from Resource class
    values = Resource::getConsommationByType();

    // Method 2: Alternative direct query (choose one)
    /*
    QSqlQuery query;
    query.prepare("SELECT TYPE, SUM(CONSOMMATION) FROM RESSOURCES GROUP BY TYPE");

    values = {0, 0, };

    if (query.exec()) {
        while (query.next()) {
            QString type = query.value(0).toString().toLower();
            double consommation = query.value(1).toDouble();

            if (type.contains("eau") || type.contains("water")) {
                values[0] = consommation;
            } else if (type.contains("électricité") || type.contains("electricity") || type.contains("elec")) {
                values[1] = consommation;
            } else {
                values[2] += consommation;
            }
        }
    } else {
        qDebug() << "Error loading statistics:" << query.lastError().text();
        // Set default values if query fails
        values = { 100, 75, 50 };
    }
    */

    // If all values are zero, show some default data
    double total = 0;
    for (double v : values) total += v;

    if (total == 0) {
        values = { 120, 45, 80 }; // Default demo values
        qDebug() << "No data found, using demo values";
    }
}

void StatWindow::setProgress(double value)
{
    m_progress = value;
    update();
}

void StatWindow::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Calculate total
    double total = 0;
    for (double v : values) total += v;

    // Draw title
    p.setPen(Qt::black);
    p.setFont(QFont("Arial", 14, QFont::Bold));
    p.drawText(rect(), Qt::AlignTop | Qt::AlignHCenter, "Consommation par Type");

    // Draw pie chart
    int size = 250;
    int x = (width() - size) / 2;
    int y = 50; // Leave space for title
    QRectF rect(x, y, size, size);

    double startAngle = 0;

    // Draw each pie slice
    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] == 0) continue; // Skip zero values

        double percent = values[i] / total;
        double spanAngle = 360.0 * percent * m_progress;

        p.setBrush(colors[i]);
        p.setPen(Qt::black);
        p.drawPie(rect, startAngle * 16, spanAngle * 16);

        startAngle += spanAngle;
    }

    // Draw legend
    drawLegend(p, y + size + 20);
}

void StatWindow::drawLegend(QPainter &p, int startY)
{
    double total = 0;
    for (double v : values) total += v;

    p.setFont(QFont("Arial", 10));
    int legendX = 50;
    int legendItemHeight = 25;

    for (int i = 0; i < values.size(); i++)
    {
        if (values[i] == 0) continue;

        double percent = (values[i] / total) * 100;

        // Color box
        p.setBrush(colors[i]);
        p.setPen(Qt::black);
        p.drawRect(legendX, startY + i * legendItemHeight, 20, 15);

        // Text
        p.setPen(Qt::black);
        QString legendText = QString("%1: %2 kWh (%3%)")
                                 .arg(labels[i])
                                 .arg(values[i], 0, 'f', 1)
                                 .arg(percent, 0, 'f', 1);

        p.drawText(legendX + 30, startY + i * legendItemHeight + 12, legendText);
    }

    // Draw total
    p.setFont(QFont("Arial", 12, QFont::Bold));
    p.drawText(rect(), Qt::AlignBottom | Qt::AlignHCenter,
               QString("Consommation Totale: %1 kWh").arg(total, 0, 'f', 1));
}
