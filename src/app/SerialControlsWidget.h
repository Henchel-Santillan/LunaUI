#ifndef PUSHUI_INCLUDE_SERIALCONTROLSWIDGET_H
#define PUSHUI_INCLUDE_SERIALCONTROLSWIDGET_H

#include <QWidget>
#include <QIODeviceBase>

class QAbstractButton;
class QButtonGroup;
class QHBoxLayout;
class QPushButton;
class QSpinBox;
class QVBoxLayout;

class SerialControlsWidget : public QWidget {
    Q_OBJECT

public:
    explicit SerialControlsWidget(QWidget *pParent = nullptr);

    void setConfigureButtonEnabled(bool enabled);
    void setStartButtonEnabled(bool enabled);
    void setEndButtonEnabled(bool enabled);
    void setSendButtonEnabled(bool enabled);

signals:
    void configureRequested();
    void startRequested(QIODeviceBase::OpenMode openMode);
    void endRequested();
    void sendRequested(int data);

private slots:
    void onRwGroupButtonClicked(QAbstractButton *pButton);
    void onStartButtonClicked();
    void onSendButtonClicked();

private:
    QVBoxLayout *m_pMainLayout;

    QPushButton *m_pConfigureButton;
    QPushButton *m_pStartRequestButton;
    QPushButton *m_pEndRequestButton;

    QPushButton *m_pSendButton;
    QSpinBox *m_pSendBox;
    QButtonGroup *m_pRwGroup;

    // Currently only support for read-only, write-only, and read-write
    QIODeviceBase::OpenMode m_openMode;
};

#endif // PUSHUI_INCLUDE_SERIALCONTROLSWIDGET_H
