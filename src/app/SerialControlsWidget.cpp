#include "SerialControlsWidget.h"

#include <QAbstractButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSizePolicy>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialControlsWidget::SerialControlsWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pConfigureButton(new QPushButton("Configure"))
    , m_pStartRequestButton(new QPushButton("Start"))
    , m_pEndRequestButton(new QPushButton("End"))
    , m_pSendButton(new QPushButton("Send"))
    , m_pSendBox(new QSpinBox)
    , m_pRwGroup(new QButtonGroup)
    , m_pButtonGroupBox(new QGroupBox("Open Mode"))
    , m_openMode(QIODeviceBase::ReadOnly)
{
    // Connect signals to propagate up to SerialViewDialog or to private slots
    QObject::connect(m_pConfigureButton, &QAbstractButton::clicked, this, &SerialControlsWidget::configureRequested);
    QObject::connect(m_pStartRequestButton, &QAbstractButton::clicked, this, &SerialControlsWidget::onStartButtonClicked);
    QObject::connect(m_pEndRequestButton, &QAbstractButton::clicked, this, &SerialControlsWidget::endRequested);
    QObject::connect(m_pSendButton, &QAbstractButton::clicked, this, &SerialControlsWidget::onSendButtonClicked);

    m_pConfigureButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Must configure a serial port first before being allowed to start a R/W operation
    m_pStartRequestButton->setEnabled(false);
    m_pStartRequestButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // A serial port must be open and actively connected to allow an end request to occur
    m_pEndRequestButton->setEnabled(false);
    m_pEndRequestButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QLabel *pSendLabel = new QLabel("0x");
    pSendLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Write mode must be selected for this button to be enabled
    // Basic validation must also be met
    m_pSendButton->setEnabled(false);
    m_pSendButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Set up the QSpinBox. Currently, only commands for LCD diagnostics are supported
    m_pSendBox->setRange(0, 255);
    m_pSendBox->setDisplayIntegerBase(16);  // Hexadecimal
    m_pSendBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_pSendBox->setEnabled(false);

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
        switch (m_pRwGroup->id(pButton)) {
        case 0:     // Read
            m_pSendBox->setEnabled(false);
            m_openMode = QIODeviceBase::ReadOnly;
            break;
        case 1:     // Write
            m_pSendBox->setEnabled(true);
            m_openMode = QIODeviceBase::WriteOnly;
            break;
        case 2:
            m_pSendBox->setEnabled(true);
            m_openMode = QIODeviceBase::ReadWrite;
            break;
        }
    });

    // Add the buttons to a QGroupBox
    QVBoxLayout *pRadioButtonLayout = new QVBoxLayout;
    pRadioButtonLayout->addWidget(pReadButton);
    pRadioButtonLayout->addWidget(pWriteButton);
    pRadioButtonLayout->addWidget(pReadWriteButton);

    m_pButtonGroupBox->setLayout(pRadioButtonLayout);

    // Set up the widget layout
    QVBoxLayout *pButtonLayout = new QVBoxLayout;
    pButtonLayout->addWidget(m_pConfigureButton);
    pButtonLayout->addWidget(m_pStartRequestButton);
    pButtonLayout->addWidget(m_pEndRequestButton);

    QHBoxLayout *pTopControls = new QHBoxLayout;
    pTopControls->addLayout(pButtonLayout);
    pTopControls->addWidget(m_pButtonGroupBox, 0, Qt::AlignVCenter);

    QHBoxLayout *pBottomControls = new QHBoxLayout;
    pBottomControls->addWidget(pSendLabel);   // Hexadecimal inputs expected
    pBottomControls->addWidget(m_pSendBox);
    pBottomControls->addWidget(m_pSendButton);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pTopControls);
    pMainLayout->addLayout(pBottomControls);

    // Set the vertical box layout to be this widget's layout
    this->setLayout(pMainLayout);
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

void SerialControlsWidget::setRwButtonGroupEnabled(bool enabled) {
    m_pButtonGroupBox->setEnabled(enabled);
}


// ========== PRIVATE SLOTS

void SerialControlsWidget::onStartButtonClicked() {
    emit(m_openMode);
}

void SerialControlsWidget::onSendButtonClicked() {
    auto data = m_pSendBox->value();
    m_pSendButton->setEnabled(false);
    emit sendRequested(data);
}
