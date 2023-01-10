#include "VideoRecentListWidget.h"

#include <QAbstractItemView>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QSizePolicy>
#include <QString>
#include <QStyle>
#include <QToolButton>
#include <QUrl>
#include <QVariant>
#include <QVBoxLayout>


// ========== CONSTANTS

static constexpr const char *TITLE = "Recent ";


// ========== CONSTRUCTOR DEFINITION

VideoRecentListWidget::VideoRecentListWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMainLayout(new QVBoxLayout)
    , m_pListWidget(new QListWidget)
    , m_pTitleLabel(new QLabel)
    , m_pDownButton(new QToolButton)
    , m_pUpButton(new QToolButton)
    , m_pClearButton(new QToolButton)
    , m_pRemoveButton(new QToolButton)
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    this->updateTitleLabel();

    // Disable all the control buttons when shown first
    this->setControlButtonsEnabled(false);

    // Set the tool button styles
    m_pDownButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown));
    m_pUpButton->setIcon(this->style()->standardIcon(QStyle::SP_ArrowUp));
    m_pClearButton->setIcon(this->style()->standardIcon(QStyle::SP_TrashIcon));
    m_pRemoveButton->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    // Connect the tool buttons to the appropriate private handlers
    QObject::connect(m_pDownButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onDownButtonClicked);
    QObject::connect(m_pUpButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onUpButtonClicked);
    QObject::connect(m_pClearButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onClearButtonClicked);
    QObject::connect(m_pRemoveButton, &QAbstractButton::clicked, this, &VideoRecentListWidget::onRemoveButtonClicked);

    // Connect the QListWidget to item activation slot
    QObject::connect(m_pListWidget, &QListWidget::itemActivated, this, &VideoRecentListWidget::onItemActivated);

    m_pListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_pListWidget->setUniformItemSizes(true);

    // Add the buttons to the horizontal box layout
    QHBoxLayout *pControlsLayout = new QHBoxLayout;
    pControlsLayout->addWidget(m_pUpButton);
    pControlsLayout->addWidget(m_pDownButton);
    pControlsLayout->addWidget(m_pRemoveButton);
    pControlsLayout->addWidget(m_pClearButton);

    // Add the items to the main layout
    m_pMainLayout->addWidget(m_pTitleLabel);
    m_pMainLayout->addWidget(m_pListWidget);
    m_pMainLayout->addLayout(pControlsLayout);

    // Set the layout of this widget to be the main layout
    this->setLayout(m_pMainLayout);
}


// ========= PUBLIC FUNCTIONS

int VideoRecentListWidget::itemCount() const {
    return m_pListWidget->count();
}


// ========== PUBLIC SLOTS

void VideoRecentListWidget::push(const QString &path, const QUrl &url) {
    auto item = new QListWidgetItem(path);
    item->setData(Qt::DisplayRole, QVariant::fromValue(url));

    // Insert the new QListWidgetItem at the beginning of the QListWidget and set the current row to the top
    m_pListWidget->insertItem(0, item);
    m_pListWidget->setCurrentRow(0);
    this->updateTitleLabel();
    this->setControlButtonsEnabled(true);
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
        this->updateTitleLabel();
        this->setControlButtonsEnabled(false);
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
        this->updateTitleLabel();
        this->setControlButtonsEnabled(m_pListWidget->count() > 0);
    }    
}

void VideoRecentListWidget::onItemActivated(QListWidgetItem *pItem) {
    QVariant item = pItem->data(Qt::DisplayRole);
    QUrl url = item.value<QUrl>();
    emit itemActivated(url);
}


// ========== PRIVATE FUNCTIONS

void VideoRecentListWidget::updateTitleLabel() {
    m_pTitleLabel->setText(QString("%1 %2")
                            .arg(QString(TITLE))
                            .arg(QString::number(m_pListWidget->count())));
}

void VideoRecentListWidget::setControlButtonsEnabled(bool enabled) {
    m_pDownButton->setEnabled(enabled);
    m_pUpButton->setEnabled(enabled);
    m_pClearButton->setEnabled(enabled);
    m_pRemoveButton->setEnabled(enabled);
}
