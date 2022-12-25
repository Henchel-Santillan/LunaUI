#ifndef PUSHUI_INCLUDE_SERIALVIEWDIALOG_H
#define PUSHUI_INCLUDE_SERIALVIEWDIALOG_H

#include "BaseDialog.h"

#include <QIODeviceBase>

class QVBoxLayout;

class SerialControlsWidget;
class SerialPortWizard;
class SerialPortReader;
class SerialPortWriter;
class SerialViewWidget;

class QSerialPort;

class SerialViewDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit SerialViewDialog(QWidget *pParent = nullptr);

private slots:
    void onConfigureRequested();
    void onStartRequested(QIODeviceBase::OpenMode openMode);
    void onEndRequested();
    void onSendRequested(int data);

private:
    SerialControlsWidget *m_pControlsWidget;
    SerialPortWizard *m_pWizard;
    SerialViewWidget *m_pViewWidget;

    SerialPortReader *m_pReader;
    SerialPortWriter *m_pWriter;

    QSerialPort *m_pSerialPort;
    QIODeviceBase::OpenMode m_openMode;
};

#endif // PUSHUI_INCLUDE_SERIALVIEWDIALOG_H
