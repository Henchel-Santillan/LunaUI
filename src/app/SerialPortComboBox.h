#ifndef LUNAUI_SERIALPORTCOMBOBOX_H
#define LUNAUI_SERIALPORTCOMBOBOX_H

#include <QComboBox>

class QVariant;

class SerialPortComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY(QVariant m_currentItemData READ currentItemData)

public:
    explicit SerialPortComboBox(QWidget *pParent = nullptr);

    QVariant currentItemData() const;

Q_SIGNALS:
    void currentItemDataChanged(QVariant data);

private Q_SLOTS:
    void onCurrentIndexChanged(int index);

private:
    QVariant m_currentItemData;
};

#endif // LUNAUI_SERIALPORTCOMBOBOX_H
