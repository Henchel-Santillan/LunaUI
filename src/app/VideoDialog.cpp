#include "VideoDialog.h"

#include "VideoControlsWidget.h"
#include "VideoRecentListWidget.h"
#include "VideoWidget.h"

#include <QAudioDevice>
#include <QAudioOutput>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaDevices>
#include <QMessageBox>
#include <QSizePolicy>
#include <QSlider>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>


// ========== CONSTANTS

static constexpr const char *DURATION_DEFAULT = "--:--:--";


// ========== CONSTRUCTOR DEFINITION

VideoDialog::VideoDialog(QWidget *pParent)
    : BaseDialog(pParent)
    , m_pControlsWidget(new VideoControlsWidget)
    , m_pRecentListWidget(new VideoRecentListWidget)
    , m_pVideoWidget(new VideoWidget)
    , m_pDeviceBox(nullptr)
    , m_pLoopCheckBox(new QCheckBox("Loop"))
    , m_pDurationLabel(new QLabel(QString(DURATION_DEFAULT)))
    , m_pDurationSlider(new QSlider(Qt::Horizontal))
    , m_pErrorLabel(new QLabel)
    , m_pStatusLabel(new QLabel)
    , m_pMediaPlayer(new QMediaPlayer(this))
    , m_pAudioOutput(new QAudioOutput(this))
    , m_duration(0)
{
    if (!m_pMediaPlayer->isAvailable()) {
        // Show an alert if it is determined that the MediaPlayer instance is not available
        QMessageBox::warning(this, "Media Player Warning", "Media Player not available.");

        // Disable controls, since there should be no items in the Recent list widget to select as sources for the Media Player
        m_pControlsWidget->setEnabled(false);
        m_pVideoWidget->setEnabled(false);
        m_pRecentListWidget->setEnabled(false);

        if (m_pDeviceBox != nullptr) {
            m_pDeviceBox->setEnabled(false);
        }
    }

    QObject::connect(m_pMediaPlayer, &QMediaPlayer::durationChanged, this, &VideoDialog::onDurationChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::errorChanged, this, &VideoDialog::onErrorChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoDialog::onMediaStatusChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::positionChanged, this, &VideoDialog::onPositionChanged);
    QObject::connect(m_pMediaPlayer, &QMediaPlayer::playbackStateChanged, this, &VideoDialog::onPlaybackStateChanged);

    // Recent List Widget
    QObject::connect(m_pRecentListWidget, &VideoRecentListWidget::itemActivated, this, &VideoDialog::onRecentListItemActivated);

    // Set up the viewer layout
    QHBoxLayout *pViewerLayout = new QHBoxLayout;
    pViewerLayout->setSpacing(5);
    pViewerLayout->setContentsMargins(0, 0, 0, 0);
    pViewerLayout->addWidget(m_pVideoWidget, 2);
    pViewerLayout->addWidget(m_pRecentListWidget);

    // Set up the info layout
    m_pErrorLabel->setWordWrap(true);
    m_pErrorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_pStatusLabel->setWordWrap(true);
    m_pStatusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    QHBoxLayout *pInfoLayout = new QHBoxLayout;
    pInfoLayout->addWidget(m_pErrorLabel);
    pInfoLayout->addWidget(m_pStatusLabel);

    // Set up the duration layout
    m_pDurationLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);

    m_pDurationSlider->setTickPosition(QSlider::NoTicks);
    m_pDurationSlider->setRange(0, m_pMediaPlayer->duration());
    m_pDurationSlider->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QObject::connect(m_pDurationSlider, &QAbstractSlider::sliderMoved, this, &VideoDialog::onDurationSliderMoved);

    QObject::connect(m_pLoopCheckBox, &QCheckBox::stateChanged, this, &VideoDialog::onLoopStateChanged);

    QHBoxLayout *pDurationLayout = new QHBoxLayout;

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    // Set up the audio output device combo box
    m_pDeviceBox = new QComboBox;
    m_pDeviceBox->addItem(QString("Default"), QVariant::fromValue(QMediaDevices::defaultAudioOutput()));

    for (const auto &device : QMediaDevices::audioOutputs()) {
        m_pDeviceBox->addItem(device.description(), QVariant::fromValue(device));
    }

    QObject::connect(m_pDeviceBox, QOverload<int>::of(&QComboBox::activated), this, &VideoDialog::onDeviceActivated);

    // Add the audio output device combo box to the horizontal box layout
    pDurationLayout->addWidget(m_pDeviceBox);
#endif

    pDurationLayout->addWidget(m_pDurationSlider);
    pDurationLayout->addWidget(m_pDurationLabel);
    pDurationLayout->addWidget(m_pLoopCheckBox);
    pDurationLayout->setContentsMargins(100, 0, 100, 0);

    // Set up the controls layout
    QVBoxLayout *pControlsLayout = new QVBoxLayout;
    pControlsLayout->addLayout(pDurationLayout);
    pControlsLayout->addWidget(m_pControlsWidget);

    // Controls widget
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::openButtonClicked, this, &VideoDialog::open);

    QObject::connect(m_pControlsWidget, &VideoControlsWidget::playPauseButtonClicked, this, &VideoDialog::onPlayPauseButtonClicked);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::stopButtonClicked, this, &VideoDialog::onStopButtonClicked);
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::stopButtonClicked, m_pVideoWidget, QOverload<>::of(&QVideoWidget::update));
    QObject::connect(m_pControlsWidget, &VideoControlsWidget::changeRate, m_pMediaPlayer, &QMediaPlayer::setPlaybackRate);

    // Set up the main layout
    QVBoxLayout *pDialogLayout = new QVBoxLayout;
    pDialogLayout->addLayout(pViewerLayout);
    pDialogLayout->addLayout(pInfoLayout);
    pDialogLayout->addLayout(pControlsLayout);

    pDialogLayout->setAlignment(pControlsLayout, Qt::AlignCenter);

    this->insertLayoutAt(0, pDialogLayout);

    // Media Player
    m_pMediaPlayer->setAudioOutput(m_pAudioOutput);
    m_pMediaPlayer->setVideoOutput(m_pVideoWidget);
}


// ========== PRIVATE SLOTS

void VideoDialog::open() {
    QFileDialog dialog;
    dialog.setWindowTitle("Open Dialog");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    dialog.setOptions(QFileDialog::ReadOnly);

    if (dialog.exec() == QDialog::Accepted) {
        auto path = dialog.selectedFiles().at(0);
        auto url = dialog.selectedUrls().at(0);

        // Add to the Recent List
        m_pRecentListWidget->push(path, url);

        // Start the MediaPlayer
        m_pMediaPlayer->setSource(url);
        m_pMediaPlayer->play();
    }
}

void VideoDialog::onDurationSliderMoved(int seconds) {
    m_pMediaPlayer->setPosition(seconds);
}

void VideoDialog::onDurationChanged(qint64 duration) {
    m_duration = duration;
    m_pDurationSlider->setMaximum(duration);
}

void VideoDialog::onErrorChanged() {
    m_pDurationLabel->setText(QString(DURATION_DEFAULT));
    if (m_pMediaPlayer->error() != QMediaPlayer::NoError) {
        // Update the error label with the QMediaPlayer Error
        m_pErrorLabel->setText("[ERROR] " + m_pMediaPlayer->errorString());
    } else {
        // Otherwise, clear it.
        m_pErrorLabel->clear();
    }
}

void VideoDialog::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    QString label("[MEDIA STATUS] ");

    switch (status) {
    case QMediaPlayer::LoadingMedia:
        label.append("Loading");
        break;
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        label.append("Buffering");
        break;
    case QMediaPlayer::StalledMedia:
        label.append("Stalled");
        break;
    case QMediaPlayer::InvalidMedia:
        label.append("Invalid Media");
        onErrorChanged();
        break;
    case QMediaPlayer::NoMedia:
        label.append("No Media");
        break;
    case QMediaPlayer::LoadedMedia:
        label.append("Loaded");
        break;
    case QMediaPlayer::EndOfMedia:
        label.append("End of Media");
        break;
    }

    m_pStatusLabel->setText(label);
}

void VideoDialog::onPlaybackStateChanged(QMediaPlayer::PlaybackState state) {
    m_pControlsWidget->onPlaybackStateChanged(state);

    QString label("[PLAYBACK STATUS] ");

    switch (state) {
    case QMediaPlayer::StoppedState:
        label.append("Media stopped");
        break;
    case QMediaPlayer::PausedState:
        label.append("Media paused");
        break;
    case QMediaPlayer::PlayingState:
        label.append("Media playing");
        break;
    }

    m_pStatusLabel->setText(label);
}

void VideoDialog::onPositionChanged(qint64 progress) {
    // Set the slider value
    if (!m_pDurationSlider->isSliderDown()) 
        m_pDurationSlider->setValue(progress);

    // Update the duration label
    qint64 durationInfo = progress / 1000;
    QString timeString;

    if (durationInfo != 0 || m_duration != 0) {
        QTime current((durationInfo / 3600) % 60, (durationInfo / 60) % 60, durationInfo % 60, (durationInfo * 1000) % 1000);
        QString timeFormat = (m_duration > 3600 /* 1 hour */) ? "hh:mm:ss" : "mm:ss";
        timeString = current.toString(timeFormat);
    }

    m_pDurationLabel->setText(timeString);
}

void VideoDialog::onDeviceActivated(int index) {
    auto device = m_pDeviceBox->itemData(index).value<QAudioDevice>();
    m_pMediaPlayer->audioOutput()->setDevice(device);
}

void VideoDialog::onRecentListItemActivated(const QUrl &url) {
    m_pMediaPlayer->setSource(url);
    m_pMediaPlayer->play();
}

void VideoDialog::onLoopStateChanged(int state) {
    switch (state) {
    case Qt::Unchecked:
        m_pMediaPlayer->setLoops(QMediaPlayer::Once);
        break;
    case Qt::Checked:
        m_pMediaPlayer->setLoops(QMediaPlayer::Infinite);
        break;
    default: break;
    }
}

void VideoDialog::onPlayPauseButtonClicked() {
    switch (m_pMediaPlayer->playbackState()) {
    case QMediaPlayer::PausedState:
        m_pMediaPlayer->play();
        break;
    case QMediaPlayer::PlayingState:
        m_pMediaPlayer->pause();
        break;
    }
}

void VideoDialog::onStopButtonClicked() {
    m_pMediaPlayer->stop();
    m_pDurationLabel->setText(QString(DURATION_DEFAULT));
    m_pMediaPlayer->setSource(QUrl());
}
