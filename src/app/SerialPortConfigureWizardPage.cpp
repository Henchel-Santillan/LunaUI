#include "SerialPortConfigureWizardPage.h"

#include "SerialPortComboBox.h"

#include <QButtonGroup>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include <QRadioButton>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariant>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialPortConfigureWizardPage::SerialPortConfigureWizardPage(QWidget *pParent)
    : QWizardPage(pParent)
    , m_pBaudRateBox(new SerialPortComboBox)
    , m_pDataBitsBox(new SerialPortComboBox)
    , m_pFlowControlBox(new SerialPortComboBox)
    , m_pParityBox(new SerialPortComboBox)
    , m_pStopBitsBox(new SerialPortComboBox)
    , m_pDirectionsGroup(new QButtonGroup)
{
    this->setTitle("Configure COM Port");
    this->setFinalPage(true);

    // Baud Rate
    QList<qint32> standardBaudRates = QSerialPortInfo::standardBaudRates();
    for (auto it = standardBaudRates.begin(); it != standardBaudRates.end(); ++it) {
        m_pBaudRateBox->addItem(QString::number(*it), QVariant::fromValue(*it));
    }

    // Data Bits Box
    m_pDataBitsBox->addItem("5 bits", QVariant::fromValue(QSerialPort::Data5));
    m_pDataBitsBox->addItem("6 bits", QVariant::fromValue(QSerialPort::Data6));
    m_pDataBitsBox->addItem("7 bits", QVariant::fromValue(QSerialPort::Data7));
    m_pDataBitsBox->addItem("8 bits (recommended)", QVariant::fromValue(QSerialPort::Data8));

    // Flow Control Box
    m_pFlowControlBox->addItem("None", QVariant::fromValue(QSerialPort::NoFlowControl));
    m_pFlowControlBox->addItem("Hardware", QVariant::fromValue(QSerialPort::HardwareControl));
    m_pFlowControlBox->addItem("Software", QVariant::fromValue(QSerialPort::SoftwareControl));

    // Parity Box
    m_pParityBox->addItem("None", QVariant::fromValue(QSerialPort::NoParity));
    m_pParityBox->addItem("Even", QVariant::fromValue(QSerialPort::EvenParity));
    m_pParityBox->addItem("Odd", QVariant::fromValue(QSerialPort::OddParity));
    m_pParityBox->addItem("Space", QVariant::fromValue(QSerialPort::SpaceParity));
    m_pParityBox->addItem("Mark", QVariant::fromValue(QSerialPort::MarkParity));

    // Stop Bits Box
    m_pStopBitsBox->addItem("One stop", QVariant::fromValue(QSerialPort::OneStop));
    m_pStopBitsBox->addItem("Two stop", QVariant::fromValue(QSerialPort::TwoStop));

#if defined(Q_OS_WIN)
    m_pStopBitsBox->addItem("One and a half stop", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif

    QFormLayout *pBoxLayout = new QFormLayout;
    pBoxLayout->addRow("Baud Rate", m_pBaudRateBox);
    pBoxLayout->addRow("Data Bits", m_pDataBitsBox);
    pBoxLayout->addRow("Flow Control", m_pFlowControlBox);
    pBoxLayout->addRow("Parity", m_pParityBox);
    pBoxLayout->addRow("Stop Bits", m_pStopBitsBox);
    pBoxLayout->setLabelAlignment(Qt::AlignLeft);

    // Directions Button Group
    QRadioButton *pAllButton = new QRadioButton("All Directions");
    QRadioButton *pInputButton = new QRadioButton("Input");
    QRadioButton *pOutputButton = new QRadioButton("Output");

    m_pDirectionsGroup->addButton(pAllButton, 0);
    m_pDirectionsGroup->addButton(pInputButton, 1);
    m_pDirectionsGroup->addButton(pOutputButton, 2);

    QVBoxLayout *pButtonLayout = new QVBoxLayout;
    pButtonLayout->addWidget(pAllButton);
    pButtonLayout->addWidget(pInputButton);
    pButtonLayout->addWidget(pOutputButton);

    QGroupBox *pButtonGroup = new QGroupBox("Direction");
    pButtonGroup->setLayout(pButtonLayout);

    // Initialize combo box positions
    m_pBaudRateBox->setCurrentIndex(0);
    m_pDataBitsBox->setCurrentIndex(0);
    m_pFlowControlBox->setCurrentIndex(0);
    m_pParityBox->setCurrentIndex(0);
    m_pStopBitsBox->setCurrentIndex(0);
    pAllButton->setChecked(true);

    // Register the combo boxes and the radio buttons as wizard fields
    this->registerField("baudRateBox", m_pBaudRateBox, "m_currentItemData", "currentItemDataChanged");
    this->registerField("dataBitsBox", m_pDataBitsBox, "m_currentItemData", "currentItemDataChanged");
    this->registerField("flowControlBox", m_pFlowControlBox, "m_currentItemData", "currentItemDataChanged");
    this->registerField("parityBox", m_pParityBox, "m_currentItemData", "currentItemDataChanged");
    this->registerField("stopBitsBox", m_pStopBitsBox, "m_currentItemData", "currentItemDataChanged");
    this->registerField("allButton", pAllButton);
    this->registerField("inputButton", pInputButton);
    this->registerField("outputButton", pOutputButton);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(new QLabel("Configure COM port baud rate, data bits, flow control, parity, stop bits, and direction."));
    pMainLayout->addLayout(pBoxLayout);
    pMainLayout->addWidget(pButtonGroup);

    this->setLayout(pMainLayout);
}
