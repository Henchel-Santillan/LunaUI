#ifndef LUNAUI_SERIALCONTROLSWIDGET_H
#define LUNAUI_SERIALCONTROLSWIDGET_H

#include <QWidget>
#include <QIODeviceBase>

class QAbstractButton;
class QButtonGroup;
class QGroupBox;
class QHBoxLayout;
class QPushButton;
class QSpinBox;

class SerialControlsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SerialControlsWidget(QWidget *pParent = nullptr);

    void setConfigureButtonEnabled(bool enabled);
    void setStartButtonEnabled(bool enabled);
    void setEndButtonEnabled(bool enabled);
    void setSendButtonEnabled(bool enabled);
    void setRwButtonGroupEnabled(bool enabled);

signals:
    void configureRequested();
    void startRequested(QIODeviceBase::OpenMode openMode);
    void endRequested();
    void sendRequested(int data);

private slots:
    void onStartButtonClicked();
    void onSendButtonClicked();

private:
    QPushButton *m_pConfigureButton;
    QPushButton *m_pStartRequestButton;
    QPushButton *m_pEndRequestButton;
    QPushButton *m_pSendButton;

    QSpinBox *m_pSendBox;
    QButtonGroup *m_pRwGroup;
    QGroupBox *m_pButtonGroupBox;

    // Currently only support for read-only, write-only, and read-write
    QIODeviceBase::OpenMode m_openMode;
};

#endif // LUNAUI_SERIALCONTROLSWIDGET_H
