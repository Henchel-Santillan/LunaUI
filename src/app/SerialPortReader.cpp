#include "SerialPortReader.h"

#include <QByteArray>
#include <QTimer>


// ========== CONSTRUCTOR DEFINITION

SerialPortReader::SerialPortReader(QObject *pParent)
    : SerialPortOpBase(pParent)
{
}


// ========== PUBLIC FUNCTIONS

void SerialPortReader::setSerialPort(QSerialPort *pPort) {
    if (pPort == nullptr) {
        if (m_pPort != nullptr) {
            QObject::disconnect(this->m_pPort, &QSerialPort::errorOccurred, this, &SerialPortReader::onError);
            QObject::disconnect(this->m_pPort, &QSerialPort::readyRead, this, &SerialPortReader::onReadyRead);
            this->m_timer.stop();
        }
    } else {
        QObject::connect(this->m_pPort, &QSerialPort::errorOccurred, this, &SerialPortReader::onError);
        QObject::connect(this->m_pPort, &QSerialPort::readyRead, this, &SerialPortReader::onReadyRead);

        this->m_timer.start(5000);
    }
}


// ========== PRIVATE SLOTS

// If an error occurred, m_pPort != nullptr
void SerialPortReader::onError(QSerialPort::SerialPortError error) {
    QString errorHeader("%1: %2");
    errorHeader = errorHeader
        .arg("[ERROR] ")
        .arg(this->m_pPort->portName())
        .arg(this->m_pPort->errorString());

    QString errorString;
    switch (error) {
    case QSerialPort::ReadError:
        errorString = "I/O :: error occurred while reading data";
        break;
    case QSerialPort::ResourceError:
        errorString = "I/O :: resource unexpectedly unavailable";
        break;
    case QSerialPort::UnsupportedOperationError:
        errorString = "PERM :: requested device not supported or prohibited";
        break;
    case QSerialPort::TimeoutError:
        errorString = "TIM :: timeout error occurred";
        break;
    case QSerialPort::UnknownError:
        errorString = "An unknown error has occurred";
        break;
    default: break;
    }

    QString errorMessage("%1 %2");
    errorMessage = errorMessage
        .arg(errorHeader)
        .arg(errorString);

    emit eventMessageReady(errorString);
}

// If the readyRead() signal was emitted, the port is open and so m_pPort != nullptr
void SerialPortReader::onReadyRead() {
    QByteArray data;
    data.append(this->m_pPort->readAll());

    if (!this->m_timer.isActive()) {
        this->m_timer.start(5000);
    }

    // If the QByteArray is empty, this either means:
    // (1) No data currently available for reading
    // (2) An error occurred
    emit eventDataReadReady(data);
}
