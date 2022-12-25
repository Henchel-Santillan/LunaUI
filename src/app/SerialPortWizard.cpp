#include "SerialPortWizard.h"

#include "SerialPortConfigureWizardPage.h"
#include "SerialPortSelectWizardPage.h"

#include <QList>


// ========== CONSTRUCTOR DEFINITION

SerialPortWizard::SerialPortWizard(QWidget *pParent)
    : QWizard(pParent)
{
    this->addPage(new SerialPortSelectWizardPage);
    this->addPage(new SerialPortConfigureWizardPage);

    // Other wizard settings
    this->setWizardStyle(QWizard::ClassicStyle);
    this->setOptions(QWizard::IgnoreSubTitles | 
                     QWizard::NoBackButtonOnStartPage | 
                     QWizard::CancelButtonOnLeft);

    QList<QWizard::WizardButton> buttonLayout;
    buttonLayout << QWizard::CancelButton 
                 << QWizard::BackButton 
                 << QWizard::NextButton
                 << QWizard::FinishButton;
    this->setButtonLayout(buttonLayout);
}
