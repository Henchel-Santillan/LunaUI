#include "SerialPortConfigurationWizardPage.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSerialPortInfo>
#include <QVariant>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialPortConfigurationWizardPage::SerialPortConfigurationWizardPage(QWidget *pParent) 
    : QWizardPage(pParent)
    , m_pMainLayout(new QVBoxLayout)
    , m_pBaudRateBox(new QComboBox)
    , m_pFlowControlBox(new QComboBox)
    , m_pParityBox(new QComboBox)
    , m_pStopBitsBox(new QComboBox)
    , m_pDirectionsGroup(new QButtonGroup)
    , m_standardBaudRates(QSerialPortInfo::standardBaudRates())
    , m_configuration({ 0, 0, 0, 0, 0 })
{
    this->setTitle("Configure COM Port");
    this->setFinalPage(true);
    this->registerField("comPortConfiguration", this, "configuration", SIGNAL(configurationChanged()));

    QLabel *pMessageLabel = new QLabel("Configure COM Port baud rate, direction, flow control, parity, and stop bits.");

    // Baud Rate ComboBox
    for (auto it = m_standardBaudRates.begin(); it != m_standardBaudRates.end(); ++it) {
        m_pBaudRateBox->addItem(QString::number(*it), QVariant::fromValue(*it));
    }

    m_pBaudRateBox->setCurrentIndex(0);

    // Flow Control Box
    m_pFlowControlBox->addItem("None", QVariant::fromValue(QSerialPort::NoFlowControl));
    m_pFlowControlBox->addItem("Hardware", QVariant::fromValue(QSerialPort::HardwareControl));
    m_pFlowControlBox->addItem("Software", QVariant::fromValue(QSerialPort::SoftwareControl));

    m_pFlowControlBox->setCurrentIndex(0);

    // Parity Box
    m_pParityBox->addItem("None", QVariant::fromValue(QSerialPort::NoParity));
    m_pParityBox->addItem("Even", QVariant::fromValue(QSerialPort::EvenParity));
    m_pParityBox->addItem("Odd", QVariant::fromValue(QSerialPort::OddParity));
    m_pParityBox->addItem("Space", QVariant::fromValue(QSerialPort::SpaceParity));
    m_pParityBox->addItem("Mark", QVariant::fromValue(QSerialPort::MarkParity));

    m_pParityBox->setCurrentIndex(0);

    // Stop Bits Box
    m_pStopBitsBox->addItem("One stop", QVariant::fromValue(QSerialPort::OneStop));
    m_pStopBitsBox->addItem("Two stop", QVariant::fromValue(QSerialPort::TwoStop));

#if defined(Q_OS_WIN)
    m_pStopBitsBox->addItem("One and a half stop", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif

    m_pStopBitsBox->setCurrentIndex(0);

    // Set up the Grid Layout
    QGridLayout *pGridLayout = new QGridLayout;

    pGridLayout->addWidget(new QLabel("Baud Rate"), 0, 0, Qt::AlignCenter);
    pGridLayout->addWidget(m_pBaudRateBox, 0, 1, Qt::AlignCenter);
    pGridLayout->addWidget(new QLabel("Flow Control"), 1, 0, Qt::AlignCenter);
    pGridLayout->addWidget(m_pFlowControlBox, 1, 1, Qt::AlignCenter);
    pGridLayout->addWidget(new QLabel("Parity"), 2, 0, Qt::AlignCenter);
    pGridLayout->addWidget(m_pParityBox, 2, 1, Qt::AlignCenter);
    pGridLayout->addWidget(new QLabel("Stop Bits"), 3, 0, Qt::AlignCenter);
    pGridLayout->addWidget(m_pStopBitsBox, 3, 1, Qt::AlignCenter);

    // Direction Button Group
    QRadioButton *pInputRadioButton = new QRadioButton("Input");
    QRadioButton *pOutputRadioButton = new QRadioButton("Output");
    QRadioButton *pAllDirsRadioButton = new QRadioButton("All Directions");

    // Set default value for Directions Button Group
    pAllDirsRadioButton->setChecked(true);    

    m_pDirectionsGroup->addButton(pInputRadioButton, 0);
    m_pDirectionsGroup->addButton(pOutputRadioButton, 1);
    m_pDirectionsGroup->addButton(pAllDirsRadioButton, 2);

    QObject::connect(m_pDirectionsGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton *pButton) {
        onDirectionsGroupButtonClicked(pButton);
    });

    QVBoxLayout *pButtonLayout = new QVBoxLayout;
    pButtonLayout->addWidget(pInputRadioButton);
    pButtonLayout->addWidget(pOutputRadioButton);
    pButtonLayout->addWidget(pAllDirsRadioButton);

    QGroupBox *pGroupBox = new QGroupBox("Direction");
    pGroupBox->setLayout(pButtonLayout);

    m_pMainLayout->addWidget(pMessageLabel);
    m_pMainLayout->addLayout(pGridLayout);
    m_pMainLayout->addWidget(pGroupBox);

    // Set the layout of the main widget 
    this->setLayout(m_pMainLayout);
}


// ========== PRIVATE SLOTS

void SerialPortConfigurationWizardPage::onBaudRateCurrentIndexChanged(int index) {
    QSerialPort::BaudRate baudRate = static_cast<QSerialPort::BaudRate>((m_pBaudRateBox->itemData(index)).toInt());
    
    // Update configuration and emit configurationChanged signal
    m_configuration.replace(0, QJsonValue(baudRate));
    emit configurationChanged();
}

void SerialPortConfigurationWizardPage::onFlowControlCurrentIndexChanged(int index) {
    QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>((m_pFlowControlBox->itemData(index)).toInt());
    
    // Update configuration and emit configurationChanged signal
    m_configuration.replace(1, QJsonValue(flowControl));
    emit configurationChanged();
}

void SerialPortConfigurationWizardPage::onParityCurrentIndexChanged(int index) {
    QSerialPort::Parity parity = static_cast<QSerialPort::Parity>((m_pParityBox->itemData(index)).toInt());
    
    // Update configuration and emit configurationChanged signal
    m_configuration.replace(2, QJsonValue(parity));
    emit configurationChanged();
}

void SerialPortConfigurationWizardPage::onStopBitsCurrentIndexChanged(int index) {
    QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>((m_pStopBitsBox->itemData(index)).toInt());
    
    // Update configuration and emit configurationChanged signal
    m_configuration.replace(3, QJsonValue(stopBits));
    emit configurationChanged();
}

void SerialPortConfigurationWizardPage::onDirectionsGroupButtonClicked(QAbstractButton *pButton) {
    QSerialPort::Direction direction;

    switch (m_pDirectionsGroup->id(pButton)) {
    case 0:     // Input
        direction = QSerialPort::Input;
        break;
    case 1:     // Output
        direction = QSerialPort::Output;
        break;
    case 2:     // All Directions
        direction = QSerialPort::AllDirections;
        break;
    }

    // Update configuration and emit configurationChanged signal
    m_configuration.replace(m_configuration.size() - 1, QJsonValue(direction));
    emit configurationChanged();
}
