#include "SerialPortComboBox.h"

#include <QVariant>


// ========== CONSTRUCTOR DEFINITION

SerialPortComboBox::SerialPortComboBox(QWidget *pParent) 
    : QComboBox(pParent)
    , m_currentItemData()
{
    QObject::connect(this, &SerialPortComboBox::currentIndexChanged, this, &SerialPortComboBox::onCurrentIndexChanged);
}


// ========== PUBLIC FUNCTIONS

QVariant SerialPortComboBox::currentItemData() const {
    return m_currentItemData;
}


// ========== PRIVATE SLOTS

void SerialPortComboBox::onCurrentIndexChanged(int index) {
    auto itemData = this->itemData(index);
    if (itemData != m_currentItemData) {
        m_currentItemData = itemData;
        emit currentItemDataChanged(m_currentItemData);
    }
}

