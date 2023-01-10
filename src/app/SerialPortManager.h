#ifndef LUNAUI_SERIALPORTMANAGER_H
#define LUNAUI_SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>

class QByteArray;

class SerialPortManager : public QObject {
    Q_OBJECT

public:
    explicit SerialPortManager(QSerialPort *pSerialPort, QObject *pParent = nullptr);

public slots:
    void write(const QByteArray &payload);

signals:
    void eventMessageReady(const QString &message);
    void eventPayloadReadReady(const QByteArray &payload);
    void eventPayloadWriteReady(qint64 bytes);

private slots:
    void onErrorOccurred(QSerialPort::SerialPortError error);
    void onReadyRead();

private:
    QSerialPort *m_pSerialPort;
};

#endif // LUNAUI_SERIALPORTMANAGER_H
