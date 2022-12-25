#ifndef PUSHUI_INCLUDE_SERIALPORTSELECTIONWIZARDPAGE_H
#define PUSHUI_INCLUDE_SERIALPORTSELECTIONWIZARDPAGE_H

#include <QWizardPage>
#include <QJsonArray>
#include <QSerialPortInfo>

class QComboBox;
class QGridLayout;
class QLabel;
class QVBoxLayout;

class SerialPortSelectionWizardPage : public QWizardPage {
    Q_OBJECT
    Q_PROPERTY(QString currentPortName MEMBER m_currentPortName)

public:
   explicit SerialPortSelectionWizardPage(QWidget *pParent = nullptr);

signals:
    void currentPortNameChanged();

private slots:
    void onCurrentIndexChanged(int index);

private:
    QVBoxLayout *m_pMainLayout;
    QGridLayout *m_pInfoLayout;
    
    QComboBox *m_pPortBox;
    QLabel *m_pSerialNumberLabel;
    QLabel *m_pDescriptionLabel;
    QLabel *m_pSystemLocationLabel;
    QLabel *m_pManufacturerLabel;
    QLabel *m_pProductIdLabel;
    QLabel *m_pVendorIdLabel;

    QList<QSerialPortInfo> m_availablePorts;
    QString m_currentPortName;
};

#endif // PUSHUI_INCLUDE_SERIALPORTSELECTIONWIZARDPAGE_H
