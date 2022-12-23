#ifndef LUNAUI_VIDEORECENTLISTWIDGET_H
#define LUNAUI_VIDEORECENTLISTWIDGET_H

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QListWidget;
class QListWidgetItem;
class QToolButton;
class QUrl;
class QVBoxLayout;

static constexpr int LIST_MAX = 20;

class VideoRecentListWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoRecentListWidget(QWidget *pParent = nullptr);

public slots:
    void push(const QString &path, const QUrl &url);

signals:
    void itemActivated(const QUrl &url);   // Emitted when an item is clicked or double-clicked by the user

private slots:
    void onDownButtonClicked();
    void onUpButtonClicked();
    void onClearButtonClicked();
    void onRemoveButtonClicked();
    void onItemActivated(QListWidgetItem *pItem);

private:
    // Functions
    void updateTitleLabel();

    // Data Members
    QVBoxLayout *m_pMainLayout;
    QHBoxLayout *m_pControlsLayout;

    QListWidget *m_pListWidget;
    QLabel *m_pTitleLabel;          // Format: Recent (Count) 

    QToolButton *m_pDownButton;
    QToolButton *m_pUpButton;
    QToolButton *m_pClearButton;
    QToolButton *m_pRemoveButton;
    QToolButton *m_pSelectButton;   // Is equivalent to item activation
};

#endif // LUNAUI_VIDEORECENTLISTWIDGET_H
