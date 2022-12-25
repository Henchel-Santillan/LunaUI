#ifndef PUSHUI_INCLUDE_SERIALPORTWRITER_H
#define PUSHUI_INCLUDE_SERIALPORTWRITER_H

#include "SerialPortOpBase.h"

class QByteArray;
class QTimer;

class SerialPortWriter : public SerialPortOpBase {
    Q_OBJECT

public:
    explicit SerialPortWriter(QObject *pParent = nullptr);

    void setSerialPort(QSerialPort *pPort) override;
    void write(const QByteArray &data);

signals:
    void eventDataWriteReady(qint64 bytes, bool complete);

protected slots:
    void onError(QSerialPort::SerialPortError error) override;

private slots:
    void onBytesWritten(qint64 bytes);

private:
    qint64 m_size;
};

#endif // PUSHUI_INCLUDE_SERIALPORTWRITER_H
