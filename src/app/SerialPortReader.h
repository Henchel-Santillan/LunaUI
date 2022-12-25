#ifndef PUSHUI_INCLUDE_SERIALPORTREADER_H
#define PUSHUI_INCLUDE_SERIALPORTREADER_H

#include "SerialPortOpBase.h"

class QByteArray;
class QSerialPort;
class QTimer;

class SerialPortReader : public SerialPortOpBase {
    Q_OBJECT

public:
    explicit SerialPortReader(QObject *pParent = nullptr);

    void setSerialPort(QSerialPort *pPort) override;

signals:
    void eventDataReadReady(const QByteArray &data);

protected slots:
    void onError(QSerialPort::SerialPortError error) override;

private slots:
    void onReadyRead();

private:
    bool m_readOk;
};

#endif // PUSHUI_INCLUDE_SERIALPORTREADER_H
