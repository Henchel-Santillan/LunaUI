#include "SerialPortSelectionWizardPage.h"

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QIODeviceBase>
#include <QJsonValue>
#include <QLabel>
#include <QList>
#include <QSerialPort>
#include <QVariant>
#include <QVBoxLayout>


// =========== CONSTRUCTOR DEFINITION

SerialPortSelectionWizardPage::SerialPortSelectionWizardPage(QWidget *pParent)
    : QWizardPage(pParent)
    , m_pMainLayout(new QVBoxLayout)
    , m_pInfoLayout(new QGridLayout)
    , m_pPortBox(new QComboBox)
    , m_pSerialNumberLabel(new QLabel)
    , m_pDescriptionLabel(new QLabel)
    , m_pSystemLocationLabel(new QLabel)
    , m_pManufacturerLabel(new QLabel)
    , m_pProductIdLabel(new QLabel)
    , m_pVendorIdLabel(new QLabel)
    , m_availablePorts(QSerialPortInfo::availablePorts())
    , m_currentPortName("")
{
    this->setTitle("COM Port Selection");
    QLabel *pMessageLabel = new QLabel;

    if (m_availablePorts.empty()) {
        pMessageLabel->setText("No available COM Ports found.");
    } else {
        pMessageLabel->setText("Confirm COM port selection.");

        // Handle the currentIndexChanged signal and register the QWizardPage as a field
        QObject::connect(m_pPortBox, &QComboBox::currentIndexChanged, this, &SerialPortSelectionWizardPage::onCurrentIndexChanged);
        
        this->registerField("comPortSelection", this, "currentPortName", SIGNAL(currentPortNameChanged()));

        // Pack the QSerialPortInfo into a QJsonArray for QVariant
        for (auto it = m_availablePorts.begin(); it != m_availablePorts.end(); ++it) {
            QJsonArray packet;
            packet.append(QJsonValue((*it).serialNumber()));        // String
            packet.append(QJsonValue((*it).description()));         // String
            packet.append(QJsonValue((*it).systemLocation()));      // String
            packet.append(QJsonValue((*it).manufacturer()));        // QString
            packet.append(QJsonValue((*it).productIdentifier()));   // quint16 <=> JSON double
            packet.append(QJsonValue((*it).vendorIdentifier()));    // quint16 <=> JSON double

            // Add the port and its info packet to the QComboBox
            m_pPortBox->addItem((*it).portName(), QVariant(packet));       
        }

        // Attempt to open a QSerialPort for each QSerialPortInfo 
        // Check the DSR (Data Set Ready) pinout signal
        // Close the port
        // Sets QComboBox index to the first instance of a ready device
        int index = 0;
        for (auto it = m_availablePorts.begin(); it != m_availablePorts.end(); ++it) {
            QSerialPort port(*it);

            if (port.open(QIODeviceBase::ReadWrite) && 
                port.pinoutSignals() & QSerialPort::DataSetReadySignal == QSerialPort::DataSetReadySignal) {
                    // Set the current index of the serial port QComboBox
                    m_pPortBox->setCurrentIndex(index);
                    port.close();
                    break;
            }

            index++;
        }

        // If all open-DSR-close checks failed, set the current index of the QComboBox to the first element
        if (index == m_availablePorts.size()) {
            m_pPortBox->setCurrentIndex(0);
        }

        // Set up the GridLayout to display additional information about the selected COM Port
        QList<QLabel*> titleLabels { new QLabel("Serial Number"), new QLabel("Description"), new QLabel("System Location"), 
                                     new QLabel("Manufacturer"), new QLabel("Product ID"), new QLabel("Vendor ID") };

        QList<QLabel*> descriptionLabels { m_pSerialNumberLabel, m_pDescriptionLabel, m_pSystemLocationLabel,
                                           m_pManufacturerLabel, m_pProductIdLabel, m_pVendorIdLabel };
        
        // The number of title labels takes precedence
        for (auto i = 0; i < titleLabels.count(); ++i) {
            descriptionLabels.at(i)->setWordWrap(true);
            m_pInfoLayout->addWidget(titleLabels.at(i), i, 0, 1, 1, Qt::AlignCenter);
            m_pInfoLayout->addWidget(descriptionLabels.at(i), i, 1);
        }
    }

    // Set up the main layout of the QWizardPage
    m_pMainLayout->addWidget(pMessageLabel);
    m_pMainLayout->addWidget(m_pPortBox);
    m_pMainLayout->addLayout(m_pInfoLayout);

    this->setLayout(m_pMainLayout);
}


// ========== PRIVATE SLOTS

void SerialPortSelectionWizardPage::onCurrentIndexChanged(int index) {
    auto portInfo = m_availablePorts.at(index);

    m_pSerialNumberLabel->setText(portInfo.serialNumber());
    m_pDescriptionLabel->setText(portInfo.description());
    m_pSystemLocationLabel->setText(portInfo.systemLocation());
    m_pManufacturerLabel->setText(portInfo.manufacturer());
    m_pProductIdLabel->setText(portInfo.hasProductIdentifier() ? QString::number(portInfo.productIdentifier()) : "None");
    m_pVendorIdLabel->setText(portInfo.hasVendorIdentifier() ? QString::number(portInfo.vendorIdentifier()) : "None");

    m_currentPortName = portInfo.portName();
    emit currentPortNameChanged();
}
