#ifndef LUNAUI_SERIALVIEWWIDGET_H
#define LUNAUI_SERIALVIEWWIDGET_H

#include <QWidget>

class QListWidget;

static constexpr int LOG_CAPACITY = 20;

class SerialViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit SerialViewWidget(QWidget *pParent = nullptr);

    void enqueue(bool isData, const QString &message);

public slots:
    void onEventMessageReady(const QString &message);
    void onEventDataReadReady(const QByteArray &data);
    void onEventDataWriteReady(qint64 bytes, bool complete);

private:
    // Functions
    bool tryTakeFromMessages();
    bool tryTakeFromData();

    // Data members
    QListWidget *m_pMessageList;
    QListWidget *m_pDataList;
};

#endif // LUNAUI_SERIALVIEWWIDGET_H
