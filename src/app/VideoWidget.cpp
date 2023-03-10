#include "VideoWidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QSizePolicy>


// ========== CONSTANTS

static constexpr int VGA_PIXELS_PER_LINE = 640;
static constexpr int VGA_LINES = 480;


// ========== CONSTRUCTOR DEFINITION

VideoWidget::VideoWidget(QWidget *pParent)
    : QVideoWidget(pParent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, Qt::black);
    this->setPalette(palette);
    this->resize(VGA_PIXELS_PER_LINE, VGA_LINES);
}


// ========== PROTECTED FUNCTIONS

void VideoWidget::keyPressEvent(QKeyEvent *pEvent) {
    if (this->isFullScreen() && (pEvent->key() == Qt::Key_Escape || pEvent->key() == Qt::Key_Back)) {
        this->setFullScreen(false);
        pEvent->accept();
    } else if (pEvent->key() == Qt::Key_Enter && pEvent->modifiers() & Qt::Key_Alt) {
        this->setFullScreen(!this->isFullScreen());
        pEvent->accept();
    } else QVideoWidget::keyPressEvent(pEvent);
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *pEvent) {
    this->setFullScreen(!this->isFullScreen());
    pEvent->accept();
}

