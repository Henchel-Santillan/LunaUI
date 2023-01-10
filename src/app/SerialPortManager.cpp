#include "SerialPortManager.h"

#include <QByteArray>


// ========== CONSTRUCTOR DEFINITION

SerialPortManager::SerialPortManager(QSerialPort *pSerialPort, QObject *pParent)
    : QObject(pParent)
    , m_pSerialPort(pSerialPort)
{
    // Connect signals
    QObject::connect(m_pSerialPort, &QSerialPort::errorOccurred, this, &SerialPortManager::onErrorOccurred);
    QObject::connect(m_pSerialPort, &QSerialPort::bytesWritten, this, &SerialPortManager::eventPayloadWriteReady);
    QObject::connect(m_pSerialPort, &QSerialPort::readyRead, this, &SerialPortManager::onReadyRead);
}


// ========== PUBLIC SLOTS

void SerialPortManager::write(const QByteArray &payload) {
    auto bytesWritten = m_pSerialPort->write(payload);

    QString writeMessage("[WRITE] %1 %2");
    writeMessage = writeMessage
        .arg(m_pSerialPort->portName())
        .arg((bytesWritten != payload.size()) ? "Failed to write data or all data to the port." : "Payload written successfully.");
    emit eventMessageReady(writeMessage);
}


// ========== PRIVATE SLOTS

void SerialPortManager::onErrorOccurred(QSerialPort::SerialPortError error) {
    QString errorMessage("[ERROR] %1 %2");
    errorMessage = errorMessage
        .arg(QString::number(m_pSerialPort->error()))
        .arg(m_pSerialPort->errorString());
    emit eventMessageReady(errorMessage);
}

void SerialPortManager::onReadyRead() {
    QByteArray payload;
    payload.append(m_pSerialPort->readAll());
    emit eventPayloadReadReady(payload);
}
