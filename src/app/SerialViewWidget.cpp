#include "SerialViewWidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QSizePolicy>
#include <QVBoxLayout>


// ========== CONSTRUCTOR DEFINITION

SerialViewWidget::SerialViewWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMessageList(new QListWidget)
    , m_pDataList(new QListWidget)
{
    QHBoxLayout *pLabelLayout = new QHBoxLayout;
    pLabelLayout->addWidget(new QLabel("Messages"));
    pLabelLayout->addWidget(new QLabel("Data"));

    QHBoxLayout *pListLayout = new QHBoxLayout;
    pListLayout->addWidget(m_pMessageList);
    pListLayout->addWidget(m_pDataList);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pLabelLayout);
    pMainLayout->addLayout(pListLayout);

    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    this->setLayout(pMainLayout);
}


// ========== PUBLIC FUNCTIONS

void SerialViewWidget::enqueue(bool isData, const QString &message) {
    if (isData) {
        tryTakeFromData();
        m_pDataList->addItem(message);
    } else {
        tryTakeFromMessages();
        m_pMessageList->addItem(message);
    }
}


// ========== PUBLIC SLOTS

void SerialViewWidget::onEventMessageReady(const QString &message) {
    this->enqueue(false, message);
}

void SerialViewWidget::onEventPayloadReadReady(const QByteArray &data) {
    QString dataString("%1 %2");
    dataString = dataString
        .arg(QString::fromStdString("[READ]"))
        .arg(QString(data));
    this->enqueue(true, dataString);
}

void SerialViewWidget::onEventPayloadWriteReady(qint64 bytes) {
    QString writeString("%1 : %2");
    writeString = writeString
        .arg(QString::fromStdString("Bytes written"))
        .arg(QString::number(bytes));

    QString dataString("%1 \n %2");
    dataString = dataString
        .arg(QString::fromStdString("WRITE]"))
        .arg(writeString);

    this->enqueue(true, dataString);
}


// ========== PRIVATE FUNCTIONS

bool SerialViewWidget::tryTakeFromMessages() {
    auto count = m_pMessageList->count();
    if (count >= LOG_CAPACITY) {
        auto message = m_pMessageList->takeItem(count - 1);
        delete message;
        message = nullptr;
        return true;
    }
    return false;
}

bool SerialViewWidget::tryTakeFromData() {
    auto count = m_pDataList->count();
    if (count >= LOG_CAPACITY) {
        auto data = m_pDataList->takeItem(count - 1);
        delete data;
        data = nullptr;
        return true;
    }
    return false;
}
