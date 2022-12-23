#ifndef LUNAUI_BASEDIALOG_H
#define LUNAUI_BASEDIALOG_H

#include <QDialog>

class QKeyEvent;
class QLayout;
class QShowEvent;
class QPushButton;
class QVBoxLayout;
class QWidget;

class BaseDialog : public QDialog {
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *pParent = nullptr);

    void insertWidgetAt(int index, QWidget *pWidget);
    void insertLayoutAt(int index, QLayout *pLayout);

protected slots:
    virtual void onBackButtonClicked();

protected:
    // Functions
    void keyPressEvent(QKeyEvent *pEvent) override;
    void showEvent(QShowEvent *pEvent) override;

    // Data members
    BaseDialog *m_pParent;
    QVBoxLayout *m_pMainLayout;
    QPushButton *m_pBackButton;
};

#endif // LUNAUI_BASEDIALOG_H
