#include "VideoControlsWidget.h"

#include <QAudio>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSizePolicy>
#include <QSlider>
#include <QString>
#include <QVariant>


// ========== CONSTRUCTOR DEFINITION

VideoControlsWidget::VideoControlsWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMainLayout(new QHBoxLayout)
    , m_pOpenButton(new QPushButton("Open"))
    , m_pStopButton(new QToolButton)
    , m_pPlayPauseButton(new QToolButton)
    , m_pRateBox(new QComboBox)
    , m_playbackState(QMediaPlayer::StoppedState)
{
    QObject::connect(m_pOpenButton, &QAbstractButton::clicked, this, &VideoControlsWidget::openButtonClicked);

    m_pStopButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));
    QObject::connect(m_pStopButton, &QAbstractButton::clicked, this, &VideoControlsWidget::stopButtonClicked);

    m_pPlayPauseButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(m_pPlayPauseButton, &QAbstractButton::clicked, this, &VideoControlsWidget::playPauseButtonClicked);

    m_pRateBox->addItem("0.5", QVariant(0.5));
    m_pRateBox->addItem("1.0", QVariant(1.0));
    m_pRateBox->addItem("2.0", QVariant(2.0));

    // Set the current rate to be x1.0 (normal speed)
    m_pRateBox->setCurrentIndex(1);
    QObject::connect(m_pRateBox, QOverload<int>::of(&QComboBox::activated), this, &VideoControlsWidget::onRateActivated);

    // Add the widgets to the horizontal box layout
    m_pMainLayout->addWidget(m_pOpenButton);
    m_pMainLayout->addWidget(m_pStopButton);
    m_pMainLayout->addWidget(m_pPlayPauseButton);
    m_pMainLayout->addWidget(m_pRateBox);

    for (auto i = 0; i < m_pMainLayout->count(); ++i) {
        auto widget = m_pMainLayout->itemAt(i)->widget();
        widget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    }

    m_pMainLayout->setContentsMargins(500, 0, 500, 0);

    // Set the horizontal box layout to be the layout of this widget
    this->setLayout(m_pMainLayout);
}


// ========== PUBLIC SLOTS

void VideoControlsWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    m_playbackState = state;

    switch (m_playbackState) {
    case QMediaPlayer::StoppedState:
        m_pStopButton->setEnabled(false);
        m_pPlayPauseButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    case QMediaPlayer::PausedState:
        m_pStopButton->setEnabled(true);
        m_pPlayPauseButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    case QMediaPlayer::PlayingState:
        m_pStopButton->setEnabled(true);
        m_pPlayPauseButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPause));
        break;
    }
}

void VideoControlsWidget::onChangeRate(qreal rate) {
    for (auto i = 0; i < m_pRateBox->count(); ++i) {
        if (qFuzzyCompare(static_cast<float>(rate), float(m_pRateBox->itemData(i).toDouble()))) {
            m_pRateBox->setCurrentIndex(i);
        }
    }
}


// ========== PRIVATE SLOTS

void VideoControlsWidget::onRateActivated() {
    emit changeRate(qreal(m_pRateBox->itemData(m_pRateBox->currentIndex()).toDouble()));
}
