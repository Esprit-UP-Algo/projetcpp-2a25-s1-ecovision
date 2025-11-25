#include "arduino_manager.h"
#include <QDebug>

ArduinoManager::ArduinoManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_port, &QSerialPort::readyRead, this, &ArduinoManager::onReadyRead);
}

ArduinoManager::~ArduinoManager()
{
    if (m_port.isOpen()) {
        m_port.close();
    }
}

bool ArduinoManager::open(const QString &portName, int baudRate)
{
    if (m_port.isOpen()) {
        m_port.close();
    }

    m_port.setPortName(portName);
    m_port.setBaudRate(baudRate);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);
    m_port.setFlowControl(QSerialPort::NoFlowControl);

    if (m_port.open(QIODevice::ReadWrite)) {
        qDebug() << "Arduino connecté sur" << portName;
        emit connectionStatusChanged(true);
        return true;
    } else {
        qDebug() << "Erreur connexion Arduino:" << m_port.errorString();
        emit errorOccurred(m_port.errorString());
        return false;
    }
}

void ArduinoManager::close()
{
    if (m_port.isOpen()) {
        m_port.close();
        emit connectionStatusChanged(false);
        qDebug() << "Arduino déconnecté";
    }
}

bool ArduinoManager::isOpen() const
{
    return m_port.isOpen();
}

void ArduinoManager::onReadyRead()
{
    m_buffer += QString::fromUtf8(m_port.readAll());

    // Traiter les lignes complètes (terminées par \n)
    int pos;
    while ((pos = m_buffer.indexOf('\n')) != -1) {
        QString line = m_buffer.left(pos).trimmed();
        m_buffer.remove(0, pos + 1);

        if (!line.isEmpty()) {
            // Parser le message RFID:UID
            if (line.startsWith("RFID:")) {
                QString uid = line.mid(5).trimmed();
                qDebug() << "Carte RFID détectée - UID:" << uid;
                emit rfidScanned(uid);
            } else {
                qDebug() << "Arduino:" << line;
            }
        }
    }
}
