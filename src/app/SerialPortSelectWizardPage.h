#ifndef LUNAUI_SERIALPORTSELECTWIZARDPAGE_H
#define LUNAUI_SERIALPORTSELECTWIZARDPAGE_H

#include <QWizardPage>
#include <QSerialPortInfo>

class QFormLayout;
class QLabel;
class SerialPortComboBox;

class SerialPortSelectWizardPage : public QWizardPage {
    Q_OBJECT

public:
    explicit SerialPortSelectWizardPage(QWidget *pParent = nullptr);

    bool isComplete() const override;

private slots:
    void onCurrentPortChanged(int index);

private:
    SerialPortComboBox *m_pPortBox;
    QLabel *m_pSerialNumberLabel,
           *m_pDescriptionLabel,
           *m_pSystemLocationLabel,
           *m_pManufacturerLabel,
           *m_pProductIdLabel,
           *m_pVendorIdLabel;

    QFormLayout *m_pInfoLayout;
    QList<QSerialPortInfo> m_availablePorts;
};

#endif // LUNAUI_SERIALPORTSELECTWIZARDPAGE_H
