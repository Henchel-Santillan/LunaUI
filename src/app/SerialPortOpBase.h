#ifndef LUNAUI_SERIALPORTOPBASE_H
#define LUNAUI_SERIALPORTOPBASE_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>

class SerialPortOpBase : public QObject {
    Q_OBJECT 

public:
    explicit SerialPortOpBase(QObject *pParent = nullptr);

    virtual void setSerialPort(QSerialPort *pPort);

signals:
    void eventMessageReady(const QString &message);

protected slots:
    virtual void onTimeout();
    virtual void onError(QSerialPort::SerialPortError error) = 0;

protected:
    QSerialPort *m_pPort;
    QTimer m_timer;
};

#endif // LUNAUI_SERIALPORTOPBASE_H
