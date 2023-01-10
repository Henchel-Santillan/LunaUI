#include "SerialPortSelectWizardPage.h"

#include "SerialPortComboBox.h"

#include <QComboBox>
#include <QFormLayout>
#include <QIODeviceBase>
#include <QLabel>
#include <QList>
#include <QSerialPort>
#include <QVariant>


// ========== CONSTRUCTOR DEFINITION

SerialPortSelectWizardPage::SerialPortSelectWizardPage(QWidget *pParent) 
    : QWizardPage(pParent)
    , m_pPortBox(new SerialPortComboBox)
    , m_pSerialNumberLabel(new QLabel)
    , m_pDescriptionLabel(new QLabel)
    , m_pSystemLocationLabel(new QLabel)
    , m_pManufacturerLabel(new QLabel)
    , m_pProductIdLabel(new QLabel)
    , m_pVendorIdLabel(new QLabel)
    , m_pInfoLayout(new QFormLayout)
    , m_availablePorts(QSerialPortInfo::availablePorts())
{
    this->setTitle("COM Port Selection");
    QLabel *pMessageLabel = new QLabel;

    if (m_availablePorts.empty()) {
        pMessageLabel->setText("No available COM ports found.");
    } else {
        pMessageLabel->setText("Confirm COM Port selection.");

        // Register the port combo box as a mandatory (*) wizard page field
        this->registerField("portBox*", m_pPortBox, "m_currentItemData", "currentItemDataChanged");

        auto openIndex = -1;

        // Handle the currentIndexChanged signal for the port combo box and populate it
        QObject::connect(m_pPortBox, &QComboBox::currentIndexChanged, this, &SerialPortSelectWizardPage::onCurrentPortChanged);
        for (auto i = 0; i < m_availablePorts.count(); ++i) {
            auto portInfo = m_availablePorts.at(i);
            m_pPortBox->addItem(portInfo.portName(), QVariant::fromValue(portInfo));

            // Attempt to open a QSerialPort for each QSerialPortInfo by checking the DSR pinout signal
            if (openIndex == -1) {
                QSerialPort port(portInfo);
                if (port.open(QIODeviceBase::ReadWrite) &&
                    port.pinoutSignals() & QSerialPort::DataSetReadySignal == QSerialPort::DataSetReadySignal) {
                        port.close();
                        openIndex = i;
                }
            }
        }

        // Set the index to the first found port if all DSR checks failed
        m_pPortBox->setCurrentIndex((openIndex != -1) ? openIndex : 0);

        QList<QString> titles { "Serial Number", "Description", "System Location", 
                                "Manufacturer", "Product ID", "Vendor ID" };

        QList<QLabel*> labels { m_pSerialNumberLabel, m_pDescriptionLabel, m_pSystemLocationLabel,
                                m_pManufacturerLabel, m_pProductIdLabel, m_pVendorIdLabel };
        
        // Set up the info form layout
        m_pInfoLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
        m_pInfoLayout->setLabelAlignment(Qt::AlignLeft);

        for (auto i = 0; i < titles.count(); ++i) {
            labels.at(i)->setWordWrap(true);
            m_pInfoLayout->addRow(titles.at(i), labels.at(i));
        }
    }

    // Set up the main layout of the wizard page
    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(pMessageLabel);
    pMainLayout->addWidget(m_pPortBox);
    pMainLayout->addLayout(m_pInfoLayout);

    this->setLayout(pMainLayout);
}


// ========== PUBLIC FUNCTIONS

bool SerialPortSelectWizardPage::isComplete() const {
    return m_pPortBox->count() != 0;
}


// ========== PRIVATE SLOTS

void SerialPortSelectWizardPage::onCurrentPortChanged(int index) {
    auto portInfo = m_pPortBox->itemData(index).value<QSerialPortInfo>();

    m_pSerialNumberLabel->setText(portInfo.serialNumber());
    m_pDescriptionLabel->setText(portInfo.description());
    m_pSystemLocationLabel->setText(portInfo.systemLocation());
    m_pManufacturerLabel->setText(portInfo.manufacturer());
    m_pProductIdLabel->setText(portInfo.hasProductIdentifier() ? QString::number(portInfo.productIdentifier()) : "None");
    m_pVendorIdLabel->setText(portInfo.hasVendorIdentifier() ? QString::number(portInfo.vendorIdentifier()) : "None");
}
