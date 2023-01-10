#ifndef LUNAUI_SERIALVIEWDIALOG_H
#define LUNAUI_SERIALVIEWDIALOG_H

#include "BaseDialog.h"

#include <QIODeviceBase>

class QVBoxLayout;

class SerialControlsWidget;
class SerialPortWizard;
class SerialPortManager;
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

    SerialPortManager *m_pPortManager;

    QSerialPort *m_pSerialPort;
    QIODeviceBase::OpenMode m_openMode;
};

#endif // LUNAUI_SERIALVIEWDIALOG_H
