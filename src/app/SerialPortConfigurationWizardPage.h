#ifndef PUSHUI_INCLUDE_SERIALPORTCONFIGURATIONWIZARDPAGE_H
#define PUSHUI_INCLUDE_SERIALPORTCONFIGURATIONWIZARDPAGE_H

#include <QWizardPage>
#include <QJsonArray>
#include <QSerialPort>

class QAbstractButton;
class QButtonGroup;
class QComboBox;
class QVBoxLayout;
class QWidget;

class SerialPortConfigurationWizardPage : public QWizardPage {
    Q_OBJECT
    Q_PROPERTY(QJsonArray configuration MEMBER m_configuration)

public:
    explicit SerialPortConfigurationWizardPage(QWidget *pParent = nullptr);

signals:
    void configurationChanged();

private slots:
    void onBaudRateCurrentIndexChanged(int index);
    void onFlowControlCurrentIndexChanged(int index);
    void onParityCurrentIndexChanged(int index);
    void onStopBitsCurrentIndexChanged(int index);
    void onDirectionsGroupButtonClicked(QAbstractButton *pButton);

private:
    QVBoxLayout *m_pMainLayout;

    QComboBox *m_pBaudRateBox;
    QComboBox *m_pFlowControlBox;
    QComboBox *m_pParityBox;
    QComboBox *m_pStopBitsBox;

    QButtonGroup *m_pDirectionsGroup;
    QList<qint32> m_standardBaudRates;

    // Format for m_configuration:
    // {
    //      QSerialPort::BaudRate
    //      QSerialPort::FlowControl
    //      QSerialPort::Parity
    //      QSerialPort::StopBits
    //      QSerialPort::Direction
    // }
    QJsonArray m_configuration;
};

#endif // PUSHUI_INCLUDE_SERIALPORTCONFIGURATIONWIZARDPAGE_H
