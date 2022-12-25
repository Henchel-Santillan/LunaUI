#include "SerialViewWidget.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QSizePolicy>


// ========== CONSTRUCTOR DEFINITION

SerialViewWidget::SerialViewWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMessageList(new QListWidget)
    , m_pDataList(new QListWidget)
{
    m_pMessageList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_pDataList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    QHBoxLayout *pMainLayout = new QHBoxLayout;
    pMainLayout->addWidget(m_pMessageList, 5, Qt::AlignCenter);
    pMainLayout->addWidget(m_pDataList, 5, Qt::AlignCenter);

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

void SerialViewWidget::onEventDataReadReady(const QByteArray &data) {
    QString dataString("%1 %2");
    dataString = dataString
        .arg(QString::fromStdString("[DATA-READ]"))
        .arg(QString(data));
    this->enqueue(true, dataString);
}

void SerialViewWidget::onEventDataWriteReady(qint64 bytes, bool complete) {
    QString completeString("%1 : %2");
    completeString = completeString
        .arg(QString::fromStdString("Completed"))
        .arg(QString::number(complete));

    QString writeString("%1 : %2");
    writeString = writeString
        .arg(QString::fromStdString("Bytes written"))
        .arg(QString::number(bytes));

    QString dataString("%1 \n %2 \n %3");
    dataString = dataString
        .arg(QString::fromStdString("DATA-WRITE]"))
        .arg(completeString)
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
