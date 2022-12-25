#include "SerialControlsWidget.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialControlsWidget::SerialControlsWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMainLayout(new QVBoxLayout)
    , m_pConfigureButton(new QPushButton("Configure"))
    , m_pStartRequestButton(new QPushButton("Start"))
    , m_pEndRequestButton(new QPushButton("End"))
    , m_pSendButton(new QPushButton("Send"))
    , m_pSendBox(new QSpinBox)
    , m_pRwGroup(new QButtonGroup)
    , m_openMode(QIODeviceBase::ReadOnly)
{
    // Connect signals to propagate up to SerialViewDialog or to private slots
    QObject::connect(m_pConfigureButton, &QAbstractButton::clicked, this, &SerialControlsWidget::configureRequested);
    QObject::connect(m_pStartRequestButton, &QAbstractButton::clicked, this, &SerialControlsWidget::onStartButtonClicked);
    QObject::connect(m_pEndRequestButton, &QAbstractButton::clicked, this, &SerialControlsWidget::endRequested);
    QObject::connect(m_pSendButton, &QAbstractButton::clicked, this, &SerialControlsWidget::onSendButtonClicked);

    // Must configure a serial port first before being allowed to start a R/W operation
    m_pStartRequestButton->setEnabled(false);

    // A serial port must be open and actively connected to allow an end request to occur
    m_pEndRequestButton->setEnabled(false);

    // Write mode must be selected for this button to be enabled
    // Basic validation must also be met
    m_pSendButton->setEnabled(false);

    // Set up the QSpinBox. Currently, only commands for LCD diagnostics are supported
    m_pSendBox->setRange(0, 255);
    m_pSendBox->setDisplayIntegerBase(16);  // Hexadecimal

    // Set up the QButtonGroup for read and write
    QRadioButton *pReadButton = new QRadioButton("Read");
    QRadioButton *pWriteButton = new QRadioButton("Write");
    QRadioButton *pReadWriteButton = new QRadioButton("Read and Write");

    // Set the default value as read
    pReadButton->setChecked(true);

    m_pRwGroup->addButton(pReadButton, 0);
    m_pRwGroup->addButton(pWriteButton, 1);
    m_pRwGroup->addButton(pReadWriteButton, 2);

    QObject::connect(m_pRwGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton *pButton) {
        onRwGroupButtonClicked(pButton);
    });

    // Add the buttons to a QGroupBox
    QVBoxLayout *pButtonLayout = new QVBoxLayout;
    pButtonLayout->addWidget(pReadButton);
    pButtonLayout->addWidget(pWriteButton);
    pButtonLayout->addWidget(pReadWriteButton);

    QGroupBox *pGroupBox = new QGroupBox("Open Mode");
    pGroupBox->setLayout(pButtonLayout);

    // Set up the widget layout
    QHBoxLayout *pTopControls = new QHBoxLayout;
    pTopControls->addWidget(m_pConfigureButton, 0, Qt::AlignVCenter);
    pTopControls->addWidget(pGroupBox, 0, Qt::AlignVCenter);
    pTopControls->addWidget(m_pStartRequestButton, 0, Qt::AlignVCenter);
    pTopControls->addWidget(m_pEndRequestButton, 0, Qt::AlignVCenter);

    QHBoxLayout *pBottomControls = new QHBoxLayout;
    pBottomControls->addWidget(new QLabel("0x"));   // Hexadecimal inputs expected
    pBottomControls->addWidget(m_pSendBox);
    pBottomControls->addWidget(m_pSendButton);

    m_pMainLayout->addLayout(pTopControls);
    m_pMainLayout->addLayout(pBottomControls);

    // Set the vertical box layout to be this widget's layout
    this->setLayout(m_pMainLayout);
}


// ========= PUBLIC FUNCTIONS

void SerialControlsWidget::setConfigureButtonEnabled(bool enabled) {
    m_pConfigureButton->setEnabled(enabled);
}

void SerialControlsWidget::setStartButtonEnabled(bool enabled) {
    m_pStartRequestButton->setEnabled(enabled);
}

void SerialControlsWidget::setEndButtonEnabled(bool enabled) {
    m_pEndRequestButton->setEnabled(enabled);
}

void SerialControlsWidget::setSendButtonEnabled(bool enabled) {
    m_pSendButton->setEnabled(enabled);
}


// ========== PRIVATE SLOTS

void SerialControlsWidget::onRwGroupButtonClicked(QAbstractButton *pButton) {
    // Enable or disable the QSpinBox and the Send button depending on whether read or write is selected
    switch (m_pRwGroup->id(pButton)) {
    case 0:     // Read
        m_pSendBox->setEnabled(true);
        m_openMode = QIODeviceBase::ReadOnly;
        break;
    case 1:     // Write
        m_pSendBox->setEnabled(false);
        m_openMode = QIODeviceBase::WriteOnly;
        break;
    case 2:
        m_pSendBox->setEnabled(true);
        m_openMode = QIODeviceBase::ReadWrite;
        break;
    }
}

void SerialControlsWidget::onStartButtonClicked() {
    emit (m_openMode);
}

void SerialControlsWidget::onSendButtonClicked() {
    auto data = m_pSendBox->value();
    m_pSendButton->setEnabled(false);
    emit sendRequested(data);
}
