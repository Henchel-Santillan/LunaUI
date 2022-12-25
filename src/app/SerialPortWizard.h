#ifndef PUSHUI_INCLUDE_SERIALPORTWIZARD_H
#define PUSHUI_INCLUDE_SERIALPORTWIZARD_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QWizard>

class SerialPortWizard : public QWizard {
    Q_OBJECT

public:
    explicit SerialPortWizard(QWidget *pParent = nullptr);
};

#endif // PUSHUI_INCLUDE_SERIALPORTSELECTIONWIZARD_H
