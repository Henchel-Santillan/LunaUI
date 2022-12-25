#ifndef LUNAUI_SERIALPORTCONFIGUREWIZARDPAGE_H
#define LUNAUI_SERIALPORTCONFIGUREWIZARDPAGE_H

#include <QWizardPage>

class QButtonGroup;
class SerialPortComboBox;

class SerialPortConfigureWizardPage : public QWizardPage {
    Q_OBJECT

public:
    explicit SerialPortConfigureWizardPage(QWidget *pParent = nullptr);

private:
    SerialPortComboBox *m_pBaudRateBox,
                       *m_pDataBitsBox,
                       *m_pFlowControlBox,
                       *m_pParityBox,
                       *m_pStopBitsBox;
    
    QButtonGroup *m_pDirectionsGroup;
};

#endif // LUNAUI_SERIALPORTCONFIGUREWIZARDPAGE_H
