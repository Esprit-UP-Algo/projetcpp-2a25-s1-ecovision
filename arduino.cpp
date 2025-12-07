#include "arduino.h"

Arduino::Arduino()
{
    data = "";
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}

QSerialPort *Arduino::getserial()
{
    return serial;
}

int Arduino::connect_arduino()
{
    // réinitialiser l'état
    arduino_is_available = false;
    arduino_port_name.clear();

    qDebug() << "=== Scanning serial ports ===";

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &serial_port_info : ports) {

        QString vidStr = QString("0x%1")
        .arg(serial_port_info.vendorIdentifier(), 4, 16, QChar('0'));
        QString pidStr = QString("0x%1")
                             .arg(serial_port_info.productIdentifier(), 4, 16, QChar('0'));

        qDebug() << "Port:" << serial_port_info.portName()
                 << "| desc:" << serial_port_info.description()
                 << "| manufacturer:" << serial_port_info.manufacturer()
                 << "| hasVID:" << serial_port_info.hasVendorIdentifier()
                 << "| hasPID:" << serial_port_info.hasProductIdentifier()
                 << "| VID:" << vidStr
                 << "| PID:" << pidStr;

        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_producy_id) {

                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << ">>> MATCH Arduino VID/PID on port" << arduino_port_name;
            }
        }
    }

    qDebug() << "arduino_port_name is:" << arduino_port_name
             << "| arduino_is_available:" << arduino_is_available;

    if (arduino_is_available) { // configuration de la communication (débit...)
        serial->setPortName(arduino_port_name);

        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);     // débit : 9600 bits/s
            serial->setDataBits(QSerialPort::Data8);        // 8 bits de données
            serial->setParity(QSerialPort::NoParity);       // pas de parité
            serial->setStopBits(QSerialPort::OneStop);      // 1 bit de stop
            serial->setFlowControl(QSerialPort::NoFlowControl);

            qDebug() << "Arduino connected successfully on" << arduino_port_name;
            return 0;
        } else {
            qDebug() << "Failed to open port" << arduino_port_name
                     << "error:" << serial->errorString();
            return 1;
        }
    }

    qDebug() << "No Arduino found with VID/PID ="
             << QString("0x%1").arg(arduino_uno_vendor_id, 4, 16, QChar('0'))
             << QString("0x%1").arg(arduino_uno_producy_id, 4, 16, QChar('0'));

    return -1;
}

int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Serial port" << arduino_port_name << "closed.";
        return 0;
    }
    qDebug() << "Serial port was not open.";
    return 1;
}

QByteArray Arduino::read_from_arduino()
{
    if (serial->isReadable()) {
        data = serial->readAll(); // récupérer les données reçues

        if (!data.isEmpty())
            qDebug() << "read_from_arduino() received:" << data;

        return data;
    }

    return QByteArray(); // rien à lire
}

void Arduino::write_to_arduino(QByteArray d)
{
    if (serial->isWritable()) {
        qDebug() << "write_to_arduino() sending:" << d;
        serial->write(d);  // envoyer des donnés vers Arduino
    } else {
        qDebug() << "Couldn't write to serial! Port not writable or not open.";
    }
}
