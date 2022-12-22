#include "VideoRecentListWidget.h"

#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QString>
#include <QStyle>
#include <QToolButton>
#include <QVariant>
#include <QVBoxLayout>


// ========== CONSTANTS

static constexpr const char *TITLE = "Recent ";


// ========== CONSTRUCTOR DEFINITION

VideoRecentListWidget::VideoRecentListWidget(QWidget *pParent) 
    : QWidget(pParent)
    , m_pMainLayout(new QVBoxLayout)
    , m_pControlsLayout(new QHBoxLayout)
    , m_pListWidget(new QListWidget)
    , m_pTitleLabel(new QLabel)
    , m_pDownButton(new QToolButton)
    , m_pUpButton(new QToolButton)
    , m_pClearButton(new QToolButton)
    , m_pRemoveButton(new QToolButton)
    , m_pSelectButton(new QToolButton)
{
    updateTitleLabel();

    // Set the tool button styles
    m_pDownButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
    m_pUpButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowUp));
    m_pClearButton->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    m_pRemoveButton->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    m_pSelectButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));

    // Connect the tool buttons to the appropriate private handlers
    QObject::connect(m_pDownButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onDownButtonClicked);
    QObject::connect(m_pUpButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onUpButtonClicked);
    QObject::connect(m_pClearButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onClearButtonClicked);
    QObject::connect(m_pRemoveButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onRemoveButtonClicked);
    QObject::connect(m_pSelectButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onSelectButtonClicked);

    // Add the buttons to the horizontal box layout
    m_pControlsLayout->addWidget(m_pUpButton);
    m_pControlsLayout->addWidget(m_pDownButton);
    m_pControlsLayout->addWidget(m_pSelectButton);
    m_pControlsLayout->addWidget(m_pRemoveButton);
    m_pControlsLayout->addWidget(m_pClearButton);

    // Add the items to the main layout
    m_pMainLayout->addWidget(m_pTitleLabel);
    m_pMainLayout->addWidget(m_pListWidget);
    m_pMainLayout->addLayout(m_pControlsLayout);

    // Set the layout of this widget to be the main layout
    this->setLayout(m_pMainLayout);
}


// ========== PUBLIC SLOTS

void VideoRecentListWidget::push(const QString &display, const QString &path) {
    auto item = new QListWidgetItem(display);
    item->setData(Qt::DisplayRole, QVariant(path));

    // Insert the new QListWidgetItem at the beginning of the QListWidget and set the current row to the top
    m_pListWidget->insertItem(0, item);
    m_pListWidget->setCurrentRow(0);
    updateTitleLabel();

    if (m_pListWidget->count() == 1) {  
        // Implies count was previously 0, enable all of the tool buttons
        m_pControlsLayout->setEnabled(true);
    }
}


// ========== PRIVATE SLOTS

void VideoRecentListWidget::onDownButtonClicked() {
    auto index = m_pListWidget->currentRow() + 1;
    m_pListWidget->setCurrentRow(index % m_pListWidget->count(), QItemSelectionModel::ClearAndSelect);
}

void VideoRecentListWidget::onUpButtonClicked() {
    auto count = m_pListWidget->count();
    auto index = m_pListWidget->currentRow() - 1 + count;
    m_pListWidget->setCurrentRow(index % count, QItemSelectionModel::ClearAndSelect);
}

void VideoRecentListWidget::onClearButtonClicked() {
    // Show an alert before proceeding with the clear
    QMessageBox alert;
    alert.setWindowTitle("Clear Recent");
    alert.setText("Are you sure you want to clear all recent videos played?");
    alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    alert.setDefaultButton(QMessageBox::Cancel);
    
    // If the user clicked 'Ok', clear the list widget and update controls
    if (alert.exec() == QMessageBox::Ok) {
        m_pListWidget->clear();
        updateTitleLabel();
        m_pControlsLayout->setEnabled(false);
    }
}

void VideoRecentListWidget::onRemoveButtonClicked() {
    // Show an alert before proceeding with the removal
    QMessageBox alert;
    alert.setWindowTitle("Remove Item");
    alert.setText("Are you sure you want to remove the selected video from the recent list?");
    alert.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    alert.setDefaultButton(QMessageBox::Cancel);

    // If the user clicked 'Ok', remove the item and update controls.
    // Note that the current row updates depending on activation.
    if (alert.exec() == QMessageBox::Ok) {
        auto item = m_pListWidget->takeItem(m_pListWidget->currentRow());
        delete item;
        item = nullptr;
        updateTitleLabel();
        m_pControlsLayout->setEnabled(m_pListWidget->count() == 0);
    }    
}

void VideoRecentListWidget::onSelectButtonClicked() {
    auto item = m_pListWidget->currentItem();   // TODO: Does currentItem's row match currentRow internally?
    auto data = item->data(Qt::DisplayRole);
    emit itemActivated(data.toString());

    // TODO: Move this item to the top of the list and set the current index to 0

}


// ========== PRIVATE FUNCTIONS

void VideoRecentListWidget::updateTitleLabel() {
    m_pTitleLabel->setText(QString("%1 %2")
                            .arg(QString(TITLE))
                            .arg(QString::number(m_pListWidget->count())));
}
