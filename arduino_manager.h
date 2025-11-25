#ifndef ARDUINO_MANAGER_H
#define ARDUINO_MANAGER_H

#include <QObject>
#include <QSerialPort>

class ArduinoManager : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoManager(QObject *parent = nullptr);
    ~ArduinoManager();

    // Ouvrir/fermer la connexion série
    bool open(const QString &portName, int baudRate = 9600);
    void close();
    bool isOpen() const;

signals:
    void rfidScanned(const QString &uid);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();

private:
    QSerialPort m_port;
    QString m_buffer;
};

#endif // ARDUINO_MANAGER_H
