#include "SerialPortWriter.h"

#include <QByteArray>
#include <QTimer>


// ========== CONSTRUCTOR DEFINITION

SerialPortWriter::SerialPortWriter(QObject *pParent) 
    : SerialPortOpBase(pParent)
    , m_size(0)
{
    this->m_timer.setSingleShot(true);
}


// ========== PUBLIC FUNCTIONS

void SerialPortWriter::setSerialPort(QSerialPort *pPort) {
    if (pPort == nullptr) {
        if (m_pPort != nullptr) {
            QObject::disconnect(this->m_pPort, &QSerialPort::bytesWritten, this, &SerialPortWriter::onBytesWritten);
            QObject::disconnect(this->m_pPort, &QSerialPort::errorOccurred, this, &SerialPortWriter::onError);
            SerialPortOpBase::setSerialPort(pPort);
        }
    } else {
        SerialPortOpBase::setSerialPort(pPort);
        QObject::connect(this->m_pPort, &QSerialPort::bytesWritten, this, &SerialPortWriter::onBytesWritten);
        QObject::connect(this->m_pPort, &QSerialPort::errorOccurred, this, &SerialPortWriter::onError);
    }
}

void SerialPortWriter::write(const QByteArray &data) {
    if (this->m_pPort == nullptr) 
        return;

    m_size = data.size();
    auto bytesWritten = this->m_pPort->write(data);

    QString message(this->m_pPort->portName());
    if (bytesWritten == -1) {
        message = message.append("Failed to write data to the port.");
    } else if (bytesWritten != m_size) {
        message = message.append("Failed to write all of the data to the port.");
    } 
    this->m_timer.start(5000);
    emit eventMessageReady(message);
}


// ========== PROTECTED SLOTS

// If an error occurred, m_pPort != nullptr
void SerialPortWriter::onError(QSerialPort::SerialPortError error) {
    QString errorHeader("%1 %2: %3");
    errorHeader = errorHeader
        .arg("[ERROR]")
        .arg(this->m_pPort->portName())
        .arg(this->m_pPort->errorString());

    QString errorString;
    switch (error) {
    case QSerialPort::WriteError:
        errorString = "I/O :: error occurred while writing data";
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


// ========== PRIVATE SLOTS

// If bytes were written, m_pPort != nullptr
void SerialPortWriter::onBytesWritten(qint64 bytes) {
    emit eventDataWriteReady(bytes, bytes == m_size);
}
