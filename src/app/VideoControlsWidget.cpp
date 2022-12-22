#include "VideoControlsWidget.h"

#include <QAudio>
#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QSlider>
#include <QVariant>


// ========== CONSTANTS

static constexpr int VOLUME_MIN = 0;
static constexpr int VOLUME_MAX = 100;


// ========== CONSTRUCTOR DEFINITION

VideoControlsWidget::VideoControlsWidget(QWidget *pParent)
    : QWidget(pParent)
    , m_pMainLayout(new QHBoxLayout)
    , m_pOpenButton(new QPushButton("Open"))
    , m_pStopButton(new QToolButton)
    , m_pPlayPauseButton(new QToolButton)
    , m_pMuteButton(new QToolButton)
    , m_pVolumeSlider(new QSlider(Qt::Horizontal))
    , m_pRateBox(new QComboBox)
    , m_playbackState(QMediaPlayer::StoppedState)
    , m_muted(false)
{
    QObject::connect(m_pOpenButton, &QAbstractButton::clicked, this, &VideoControlsWidget::openButtonClicked);

    m_pStopButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));
    QObject::connect(m_pStopButton, &QAbstractButton::clicked, this, &VideoControlsWidget::stop);

    m_pPlayPauseButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(m_pPlayPauseButton, &QAbstractButton::clicked, this, &VideoControlsWidget::onPlayPauseButtonClicked);

    m_pMuteButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaVolume));
    QObject::connect(m_pMuteButton, &QAbstractButton::clicked, this, &VideoControlsWidget::onMuteButtonClicked);

    // Set up the volume slider and the playback rate combo box
    m_pVolumeSlider->setRange(VOLUME_MIN, VOLUME_MAX);
    QObject::connect(m_pVolumeSlider, QAbstractSlider::valueChanged, this, &VideoControlsWidget::onVolumeSliderValueChanged);

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
    m_pMainLayout->addWidget(m_pMuteButton);
    m_pMainLayout->addWidget(m_pVolumeSlider);
    m_pMainLayout->addWidget(m_pRateBox);

    // Set the horizontal box layout to be the layout of this widget
    this->setLayout(m_pMainLayout);
}


// ========== PUBLIC SLOTS

void VideoControlsWidget::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    if (m_playbackState == state) 
        return;
    
    switch (state) {
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

    m_playbackState = state;
}

void VideoControlsWidget::onChangeVolume(float volume) {
    qreal volumeLog = QAudio::convertVolume(volume, QAudio::LinearVolumeScale, QAudio::LogarithmicVolumeScale);
    m_pVolumeSlider->setValue(qRound(volumeLog * VOLUME_MAX));
}

void VideoControlsWidget::onChangeMuted(bool muted) {
    if (this->m_muted == muted) {
        m_pMuteButton->setIcon(this->style()->standardIcon(muted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
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

void VideoControlsWidget::onPlayPauseButtonClicked() {
    switch (m_playbackState) {
    case QMediaPlayer::StoppedState:
        emit stop();
        break;
    case QMediaPlayer::PausedState:
        emit pause();
        break;
    case QMediaPlayer::PlayingState:
        emit play();
        break;
    }
}

void VideoControlsWidget::onMuteButtonClicked() {
    emit changeMuted(!m_muted); // Use logical NOT since clicking the button should mute if unmuted, or unmute if muted
}

void VideoControlsWidget::onVolumeSliderValueChanged() {
    emit changeVolume(QAudio::convertVolume(m_pVolumeSlider->value() / qreal(VOLUME_MAX), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale));
}

void VideoControlsWidget::onRateActivated() {
    emit changeRate(qreal(m_pRateBox->itemData(m_pRateBox->currentIndex()).toDouble()));
}
