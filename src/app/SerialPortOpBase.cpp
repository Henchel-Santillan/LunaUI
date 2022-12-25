#include "SerialPortOpBase.h"


// ========== CONSTRUCTOR DEFINITION

SerialPortOpBase::SerialPortOpBase(QObject *pParent)
    : QObject(pParent) 
    , m_pPort(nullptr)
    , m_timer(QTimer(this))
{
    QObject::connect(&m_timer, &QTimer::timeout, this, &SerialPortOpBase::onTimeout);
}


// ========== PUBLIC FUNCTIONS

void SerialPortOpBase::setSerialPort(QSerialPort *pPort) {
    m_pPort = pPort;
}


// ========== PROTECTED SLOTS

void SerialPortOpBase::onTimeout() {
    if (m_pPort != nullptr) {
        QString timeoutMessage("%1 %2: %3");
        timeoutMessage = timeoutMessage
            .arg("[TIMEOUT]")
            .arg(m_pPort->portName())
            .arg(m_pPort->errorString());
            
        emit eventMessageReady(timeoutMessage);
    }
}
