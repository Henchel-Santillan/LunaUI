#include "BaseDialog.h"

#include <QAbstractButton>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QPaintEvent>
#include <QPushButton>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QWidget>


// ========== CONSTRUCTOR DEFINITION

BaseDialog::BaseDialog(QWidget *pParent) 
    : m_pParent(dynamic_cast<BaseDialog*>(pParent))
    , m_pMainLayout(new QVBoxLayout)
    , m_pBackButton(new QPushButton("Back"))
{
    m_pBackButton->setEnabled(pParent != nullptr);
    m_pBackButton->setVisible(pParent != nullptr);

    // Connect the clicked signal to the clicked slot regardless of the back button visibility
    QObject::connect(m_pBackButton, &QAbstractButton::clicked, this, &BaseDialog::onBackButtonClicked);

    // Add the back button to a footer at the bottom of the dialog, positioned to the left
    QHBoxLayout *pFooterLayout = new QHBoxLayout;
    pFooterLayout->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    pFooterLayout->addWidget(m_pBackButton, 0, Qt::AlignLeft | Qt::AlignBottom);

    m_pMainLayout->addLayout(pFooterLayout);

    this->setLayout(m_pMainLayout);
}


// ==========  PUBLIC FUNCTIONS

void BaseDialog::insertWidgetAt(const int index, QWidget *pWidget) {
    if (pWidget != nullptr && index < m_pMainLayout->count()) {
        // Add the widget to the main layout.
        // The count check ensures that the back button footer layout remains at the bottom of the dialog.
        m_pMainLayout->insertWidget(index, pWidget);
    }
}

void BaseDialog::insertLayoutAt(const int index, QLayout *pLayout) {
    if (pLayout != nullptr && index < m_pMainLayout->count()) {
        // Add the layout to the main layout.
        // The count check ensures that the back button footer layout remains at the bottom of the dialog.
        m_pMainLayout->insertLayout(index, pLayout);
    }
}


// ========== PROTECTED SLOTS

void BaseDialog::onBackButtonClicked() {
    if (m_pParent != nullptr) {
        // Hide the current dialog and raise the parent dialog
        this->lower();
        this->reject();

        m_pParent->show();
    }
}


// ========== PROTECTED FUNCTIONS

void BaseDialog::keyPressEvent(QKeyEvent *pEvent) {
    switch (pEvent->key()) {
    case Qt::Key_Escape:
        this->onBackButtonClicked();
        break;
    default:
        QDialog::keyPressEvent(pEvent);
        break;
    }
}

void BaseDialog::showEvent(QShowEvent *pEvent) {
    QDialog::showEvent(pEvent);
    this->raise();
    this->activateWindow();
}
