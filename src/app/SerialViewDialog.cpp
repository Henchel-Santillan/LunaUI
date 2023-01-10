#include "SerialViewDialog.h"

#include "SerialControlsWidget.h"
#include "SerialPortWizard.h"
#include "SerialPortManager.h"
#include "SerialViewWidget.h"

#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QSizePolicy>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialViewDialog::SerialViewDialog(QWidget *pParent)
    : BaseDialog(pParent)
    , m_pControlsWidget(new SerialControlsWidget(this))
    , m_pWizard(new SerialPortWizard(this))
    , m_pViewWidget(new SerialViewWidget(this))
    , m_pSerialPort(new QSerialPort(this))
    , m_pPortManager(new SerialPortManager(m_pSerialPort, this))
    , m_openMode()
{
    // Connect signals to appropriate handlers
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::configureRequested, this, &SerialViewDialog::onConfigureRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::startRequested, this, &SerialViewDialog::onStartRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::endRequested, this, &SerialViewDialog::onEndRequested);
    QObject::connect(m_pControlsWidget, &SerialControlsWidget::sendRequested, this, &SerialViewDialog::onSendRequested);

    QObject::connect(m_pPortManager, &SerialPortManager::eventMessageReady, m_pViewWidget, &SerialViewWidget::onEventMessageReady);
    QObject::connect(m_pPortManager, &SerialPortManager::eventPayloadReadReady, m_pViewWidget, &SerialViewWidget::onEventPayloadReadReady);
    QObject::connect(m_pPortManager, &SerialPortManager::eventPayloadWriteReady, m_pViewWidget, &SerialViewWidget::onEventPayloadWriteReady);

    // Prepare the dialog layout
    QVBoxLayout *pDialogLayout = new QVBoxLayout;
    pDialogLayout->addWidget(m_pViewWidget);
    pDialogLayout->addWidget(m_pControlsWidget);

    pDialogLayout->setAlignment(Qt::AlignCenter);
    pDialogLayout->setContentsMargins(150, 0, 150, 0);

    this->insertLayoutAt(0, pDialogLayout);
}


// ========== PRIVATE SLOTS

void SerialViewDialog::onConfigureRequested() {
    // Show the configuration wizard and check for the resulting DialogCode
    if (m_pWizard->exec() == QDialog::Accepted) {
        // Obtain the port name and construct a QSerialPortInfo object using it
        QSerialPortInfo portInfo = m_pWizard->field("portBox").value<QSerialPortInfo>();

        // Construct a QSerialPort object using the QSerialPortInfo
        m_pSerialPort->setPort(portInfo);

        // TODO FIXME: Check if default configuration is requested in the wizard (via a QCheckBox field). If it is, do not apply any of the settings below
        // before calling open().

        // Obtain the configuration values
        QSerialPort::BaudRate baudRate = m_pWizard->field("baudRateBox").value<QSerialPort::BaudRate>();
        QSerialPort::DataBits dataBits = m_pWizard->field("dataBitsBox").value<QSerialPort::DataBits>();
        QSerialPort::FlowControl flowControl = m_pWizard->field("flowControlBox").value<QSerialPort::FlowControl>();
        QSerialPort::Parity parity = m_pWizard->field("parityBox").value<QSerialPort::Parity>();
        QSerialPort::StopBits stopBits = m_pWizard->field("stopBitsBox").value<QSerialPort::StopBits>();

        QList<QVariant> directionVariants = { m_pWizard->field("allButton"), 
                                              m_pWizard->field("inputButton"),
                                              m_pWizard->field("outputButton") }; 
        
        auto index = 0;
        for (auto i = 0; i < directionVariants.count(); ++i) {
            if (directionVariants.at(i).toBool()) {
                index = i;
                break;
            }
        }

        QSerialPort::Direction direction;
        switch (index) {
        case 0:
            direction = QSerialPort::AllDirections;
            break;
        case 1:
            direction = QSerialPort::Input;
            break;
        case 2:
            direction = QSerialPort::Output;
            break;
        }

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
    m_pControlsWidget->setRwButtonGroupEnabled(false);

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

        m_pControlsWidget->setSendButtonEnabled(m_openMode == QIODeviceBase::WriteOnly || m_openMode == QIODeviceBase::ReadWrite);

        // The port was opened successfully. This means that whenever the readyRead() signal is emitted, 
        // the SerialPortReader will readAll() available data given the correct open mode (ReadOnly or ReadWrite).
        // The above applies once the serial port has been set.
    }
}

void SerialViewDialog::onEndRequested() {
    // Close the port connection. Assumes the connection is already open.
    m_pSerialPort->close();

    m_pControlsWidget->setStartButtonEnabled(true);
    m_pControlsWidget->setRwButtonGroupEnabled(true);
    m_pControlsWidget->setEndButtonEnabled(false);
}

void SerialViewDialog::onSendRequested(int data) {
    // Write to the port. Assumes that the data sent over the signal is valid.
    QByteArray payload;
    QDataStream stream(&payload, m_openMode);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << data;

    m_pPortManager->write(payload);
    m_pControlsWidget->setSendButtonEnabled(true);
}
