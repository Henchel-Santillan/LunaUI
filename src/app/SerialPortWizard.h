#ifndef LUNAUI_SERIALPORTWIZARD_H
#define LUNAUI_SERIALPORTWIZARD_H

#include <QWizard>

class SerialPortWizard : public QWizard {
    Q_OBJECT

public:
    explicit SerialPortWizard(QWidget *pParent = nullptr);
};

#endif // LUNAUI_SERIALPORTSELECTIONWIZARD_H
