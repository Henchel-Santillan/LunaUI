#include "SerialViewDialog.h"

#include "SerialControlsWidget.h"
#include "SerialPortWizard.h"
#include "SerialPortReader.h"
#include "SerialPortWriter.h"
#include "SerialViewWidget.h"

#include <QByteArray>
#include <QDataStream>
#include <QJsonArray>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialViewDialog::SerialViewDialog(QWidget *pParent)
    : BaseDialog(pParent)
    , m_pControlsWidget(new SerialControlsWidget(this))
    , m_pWizard(new SerialPortWizard(this))
    , m_pViewWidget(new SerialViewWidget(this))
    , m_pReader(new SerialPortReader(this))
    , m_pWriter(new SerialPortWriter(this))
    , m_pSerialPort(nullptr)
    , m_openMode()
{
    // Connect signals to appropriate handlers
    // Controls widget
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::configureRequested, this, &SerialViewDialog::onConfigureRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::startRequested, this, &SerialViewDialog::onStartRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::endRequested, this, &SerialViewDialog::onEndRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::sendRequested, this, &SerialViewDialog::onSendRequested);

    // Port reader and writer
    QObject::connect(m_pReader, &SerialPortReader::eventMessageReady, m_pViewWidget, &SerialViewWidget::onEventMessageReady);
    QObject::connect(m_pWriter, &SerialPortWriter::eventMessageReady, m_pViewWidget, &SerialViewWidget::onEventMessageReady);

    QObject::connect(m_pReader, &SerialPortReader::eventDataReadReady, m_pViewWidget, &SerialViewWidget::onEventDataReadReady);
    QObject::connect(m_pWriter, &SerialPortWriter::eventDataWriteReady, m_pViewWidget, &SerialViewWidget::onEventDataWriteReady);

    // Prepare the dialog layout

}


// ========== PRIVATE SLOTS

void SerialViewDialog::onConfigureRequested() {
    // Show the configuration wizard and check for the resulting DialogCode
    if (m_pWizard->exec() == QDialog::Accepted) {
        // Obtain the port name and construct a QSerialPortInfo object using it
        QString portName = (m_pWizard->field("comPortSelection")).toString();
        QSerialPortInfo portInfo(portName);

        // Construct a QSerialPort object using the QSerialPortInfo
        m_pSerialPort = new QSerialPort(portInfo, this);

        // Obtain the configuration values
        QJsonArray configuration = (m_pWizard->field("comPortConfiguration")).toJsonArray();
        QSerialPort::BaudRate baudRate = static_cast<QSerialPort::BaudRate>(configuration.at(0).toInt());
        QSerialPort::DataBits dataBits = QSerialPort::Data8;
        QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(configuration.at(1).toInt());
        QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(configuration.at(2).toInt());
        QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(configuration.at(3).toInt());
        QSerialPort::Direction direction = static_cast<QSerialPort::Direction>(configuration.at(4).toInt());

        m_pSerialPort->setBaudRate(baudRate, direction);
        m_pSerialPort->setDataBits(dataBits);
        m_pSerialPort->setFlowControl(flowControl);
        m_pSerialPort->setParity(parity);
        m_pSerialPort->setStopBits(stopBits);

        // Enable the start button in the controls widget
        m_pControlsWidget->setStartButtonEnabled(true);
    }
}

void SerialViewDialog::onStartRequested(QIODeviceBase::OpenMode openMode) {
    m_openMode = openMode;
    m_pControlsWidget->setStartButtonEnabled(false);

    // Attempt to open the SerialPort with the given open mode
    bool opened = m_pSerialPort->open(openMode);
    if (!opened) { 
        // Grab error code and error string to write message to message list
        QString errorMessage("%1: %2 %3");
        errorMessage = errorMessage
            .arg("[ERROR]: Error Code ")
            .arg(QString::number(m_pSerialPort->error()))
            .arg(m_pSerialPort->errorString());

        m_pViewWidget->enqueue(false, errorMessage);
        m_pControlsWidget->setStartButtonEnabled(true);

        // Show a QMessageBox with the above information as well
        QMessageBox alert;
        alert.setText(errorMessage);
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.exec();

    } else {
        m_pControlsWidget->setEndButtonEnabled(true);

        // The port was opened successfully. This means that whenever the readyRead() signal is emitted, 
        // the SerialPortReader will readAll() available data.
        // The above applies once the serial port has been set. Do this for both the reader and writer. 
        m_pReader->setSerialPort(m_pSerialPort);
        m_pWriter->setSerialPort(m_pSerialPort);
    }
}

void SerialViewDialog::onEndRequested() {
    // Close the port connection. Assumes the connection is already open.
    m_pSerialPort->close();

    m_pControlsWidget->setStartButtonEnabled(true);
    m_pControlsWidget->setEndButtonEnabled(false);
}

void SerialViewDialog::onSendRequested(int data) {
    // Write to the port. Assumes that the data sent over the signal is valid.
    QByteArray payload;
    QDataStream stream(&payload, m_openMode);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << data;

    m_pSerialPort->write(payload);
    m_pControlsWidget->setSendButtonEnabled(true);
}
